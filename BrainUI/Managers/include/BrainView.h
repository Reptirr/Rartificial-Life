#pragma once

#include <Dendrite.h>
#include <QGraphicsView>
#include <Storage.h>
#include <QWheelEvent>
#include <UIBatcher.h>
#include <UIController.h>
#include <UIMapper.h>
#include <QThread>
#include <QTimer>

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
            batcher_.applyBatch();
        });
        ui_timer_.start(16);
    }

    void onCreate(const std::string &id) {
        if (auto *neuron = storage_.get<Neuron>(id)) {
            // тут особый случай: надо заявить еще и аксон с дендритом(если не инпут)
            auto *dendrite = storage_.get<Dendrite>(neuron->getDendrite()); // when there is input neuron it is null
            auto *axon = storage_.get<Axon>(neuron->getAxon());

            batcher_.onCreate(
                id,
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
        } else if (auto *synaps = storage_.get<Synaps>(id)) {
            batcher_.onCreate(id, mapper_.convertSynaps(synaps));
        } else if (auto *protein = storage_.get<Protein>(id)) {
            batcher_.onCreate(id, mapper_.convertProtein(protein));
        } else {
            my_assert(false);
        }
    }

    void onUpdate(const std::string &id) {
        std::unique_ptr<Data> data;

        if (auto *neuron = storage_.get<Neuron>(id)) {
            data = mapper_.convertNeuron(neuron);
        } else if (auto *axon = storage_.get<Axon>(id)) {
            data = mapper_.convertAxon(axon);
        } else if (auto *dendrite = storage_.get<Dendrite>(id)) {
            data = mapper_.convertDendrite(dendrite);
        } else if (auto *synaps = storage_.get<Synaps>(id)) {
            data = mapper_.convertSynaps(synaps);
        } else if (auto *protein = storage_.get<Protein>(id)) {
            data = mapper_.convertProtein(protein);
        } else {
            // fallback
            return;
        }

        batcher_.onUpdate(id, std::move(data));
    }
};
