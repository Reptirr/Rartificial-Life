#pragma once

#include <Dendrite.h>
#include <QGraphicsView>
#include <Storage.h>
#include <QWheelEvent>
#include <UIBatcher.h>
#include <UIController.h>
#include <UIMapper.h>
#include <QThread>

class BrainView : public QGraphicsView {
    QGraphicsScene *scene_ = new QGraphicsScene(this);
    Storage &storage_;
    UIStorage ui_storage_{};
    UIController controller_{ui_storage_, scene_};
    UIBatcher batcher_{controller_, ui_storage_};
    UIMapper mapper_;

    QThread ui_thread;
    QTimer ui_timer_;

    void fitAll() {
        fitInView(QRectF(-width(), -height(), width()*2, height()*2), Qt::IgnoreAspectRatio);
        baseScale = transform().m11();
    }
    bool autoFit = true;
    double baseScale = 1.0;

    bool dragging_{};

protected:
    void wheelEvent(QWheelEvent* e) override {
        autoFit = false;

        const double factor = e->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;

        // ограничение зума относительно "вписано целиком"
        double next = transform().m11() * factor;
        if (next < baseScale * 0.0005 || next > baseScale * 100) return;

        // зум к курсору (работает и когда скроллбаров нет)
        QPointF before = mapToScene(e->position().toPoint());
        scale(factor, factor);
        QPointF after = mapToScene(e->position().toPoint());
        translate(after.x() - before.x(), after.y() - before.y());

        qDebug() << "zoom: " << next;
    }
    void resizeEvent(QResizeEvent* e) override {
        QGraphicsView::resizeEvent(e);
        if (autoFit) fitAll();
    }
    void showEvent(QShowEvent* e) override {
        QGraphicsView::showEvent(e);
        if (autoFit) fitAll();
    }
    void mouseDoubleClickEvent(QMouseEvent* e) override {
        // двойной клик по пустому месту: сброс зума
        autoFit = true;
        fitAll();
        QGraphicsView::mouseDoubleClickEvent(e);
    }
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            dragging_ = true;
            // setRenderHint(QPainter::Antialiasing, false);
        }
        QGraphicsView::mousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        QGraphicsView::mouseReleaseEvent(event);

        if (event->button() == Qt::LeftButton) {
            dragging_ = false;
            // setRenderHint(QPainter::Antialiasing, true);
        }
        batcher_.applyBatch();
    }

public:
    BrainView(QWidget *parent, Storage &storage, int w, int h)
        : QGraphicsView(parent),
          storage_(storage), mapper_(w, h) {
        setRenderHints(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setResizeAnchor(AnchorViewCenter);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(ScrollHandDrag);
        setViewportUpdateMode(BoundingRectViewportUpdate);
        setOptimizationFlags(DontSavePainterState
                             | DontAdjustForAntialiasing);
        scene_->setItemIndexMethod(QGraphicsScene::NoIndex);

        setFixedSize(w, h);
        scene_->setSceneRect(-w, -h, w*2, h*2);

        setScene(scene_);

        connect(&ui_timer_, &QTimer::timeout, [&] {
            if (!dragging_) batcher_.applyBatch();
        });
        ui_timer_.start(16);
    }

    void onNewNeuron(const std::string &neuron_id) {
        auto *neuron = storage_.get<Neuron>(neuron_id);
        auto *dendrite = storage_.get<Dendrite>(neuron->getDendrite()); // when there is input neuron it is null
        auto *axon = storage_.get<Axon>(neuron->getAxon());

        batcher_.onCreate(
            neuron_id,
            mapper_.convertNeuron(neuron)
        );
        if (neuron->getType() != NeuronType::INPUT) { // because input neuron does not have dendrite
            batcher_.onCreate(
                neuron->getDendrite(),
                mapper_.convertDendrite(dendrite)
            );
        }
        batcher_.onCreate(
            neuron->getAxon(),
            mapper_.convertAxon(axon)
        );
    }

    void onNewSynaps(const std::string &synaps_id) {
        auto *synaps = storage_.get<Synaps>(synaps_id);

        batcher_.onCreate(synaps_id, mapper_.convertSynaps(synaps));
    }

    void onUpdateAxon(const std::string &axon_id) {
        auto *axon = storage_.get<Axon>(axon_id);

        batcher_.onUpdate(axon_id, mapper_.convertAxon(axon));
    }

    void onUpdateDendrite(const std::string &dendrite_id) {
        auto *dendrite = storage_.get<Dendrite>(dendrite_id);

        batcher_.onUpdate(dendrite_id, mapper_.convertDendrite(dendrite));
    }
};
