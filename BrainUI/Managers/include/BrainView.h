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

    QThread ui_thread;
    QTimer ui_timer_;

    void fitAll() {
        fitInView(QRectF(-1, -1, 2, 2), Qt::IgnoreAspectRatio);
        baseScale = transform().m11();
    }
    bool autoFit = true;
    double baseScale = 1.0;

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

public:
    BrainView(QWidget *parent, Storage &storage)
        : QGraphicsView(parent),
          storage_(storage) {
        setRenderHints(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setResizeAnchor(AnchorViewCenter);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setDragMode(ScrollHandDrag);

        scene_->setSceneRect(-10, -10, 20, 20);
        setScene(scene_);

        connect(&ui_timer_, &QTimer::timeout, [&] {
            batcher_.applyBatch();
        });
        ui_timer_.start(16);
    }

    void onNewNeuron(const std::string &neuron_id) {
        auto *neuron = storage_.get<Neuron>(neuron_id);
        auto *dendrite = storage_.get<Dendrite>(neuron->getDendrite());
        auto *axon = storage_.get<Axon>(neuron->getAxon());

        batcher_.onCreate(
            neuron_id,
            UIMapper::convertNeuron(neuron)
        );
        batcher_.onCreate(
            neuron->getDendrite(),
            UIMapper::convertDendrite(dendrite)
        );
        batcher_.onCreate(
            neuron->getAxon(),
            UIMapper::convertAxon(axon)
        );
    }

    void onNewSynaps(const std::string &synaps_id) {
        auto *synaps = storage_.get<Synaps>(synaps_id);

        batcher_.onCreate(synaps_id, UIMapper::convertSynaps(synaps));
    }

    void onUpdateAxon(const std::string &axon_id) {
        auto *axon = storage_.get<Axon>(axon_id);

        batcher_.onUpdate(axon_id, UIMapper::convertAxon(axon));
    }

    void onUpdateDendrite(const std::string &dendrite_id) {
        auto *dendrite = storage_.get<Dendrite>(dendrite_id);

        batcher_.onUpdate(dendrite_id, UIMapper::convertDendrite(dendrite));
    }
};
