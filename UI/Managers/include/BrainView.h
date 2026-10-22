#pragma once

#include <Dendrite.h>
#include <DendriteItem.h>
#include <NeuronItem.h>
#include <QGraphicsView>
#include <Storage.h>
#include <SynapsItem.h>
#include <QWheelEvent>
#include <UIController.h>
#include <UIMapper.h>

class BrainView : public QGraphicsView {
    QGraphicsScene *scene_ = new QGraphicsScene(this);
    Storage &storage_;
    UIStorage ui_storage_{};
    UIController controller_{ui_storage_, scene_};

    void fitAll() {
        fitInView(QRectF(-1, -1, 2, 2), Qt::IgnoreAspectRatio);
        baseScale = transform().m11();
    }
    bool autoFit = true;
    double baseScale = 1.0;

protected:
    void wheelEvent(QWheelEvent* e) override {
        autoFit = false;

        double factor = e->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;

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
    }

    void updateAll() const {
        for (auto *item : scene_->items()) {
            if (auto *component = dynamic_cast<UIComponent *>(item)) {
                component->UIUpdate();
            }
        }
    }

    void onNewNeuron(const std::string &neuron_id) const {
        auto *neuron = storage_.get<Neuron>(neuron_id);
        auto *dendrite = storage_.get<Dendrite>(neuron->getDendrite());
        auto *axon = storage_.get<Axon>(neuron->getAxon());

        controller_.newNeuron(
            neuron_id,
            UIMapper::convertNeuron(neuron),
            UIMapper::convertDendrite(dendrite),
            UIMapper::convertAxon(axon)
        );
    }

    void onNewSynaps(const std::string &synaps_id) {
        auto *synaps = storage_.get<Synaps>(synaps_id);

        controller_.newSynaps(
            synaps_id,
            UIMapper::convertSynaps(synaps)
        );
    }

    void onUpdateAxon(const std::string &axon_id) {
        auto *axon = storage_.get<Axon>(axon_id);

        controller_.updateAxon(
            axon_id,
            UIMapper::convertAxon(axon)
        );
    }

    void onUpdateDendrite(const std::string &dendrite_id) {
        auto *dendrite = storage_.get<Dendrite>(dendrite_id);

        controller_.updateDendrite(
            dendrite_id,
            UIMapper::convertDendrite(dendrite)
        );
    }
};
