#pragma once

#include <Dendrite.h>
#include <DendriteItem.h>
#include <NeuronItem.h>
#include <QGraphicsView>
#include <Storage.h>
#include <SynapsItem.h>
#include <QWheelEvent>

class BrainView : public QGraphicsView {
    Storage &storage_;
    QGraphicsScene *scene_;

    std::unordered_map<std::string, UIComponent *> items_;

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

        scene_ = new QGraphicsScene(this);
        scene_->setSceneRect(-10, -10, 20, 20);
        setScene(scene_);
    }

    void updateAll() {
        for (auto [id, component] : items_) {
            component->UIUpdate();
        }
    }

    void onNewNeuron(const std::string &neuron_id, const std::string &dendrite_id, const std::string &axon_id) {
        qDebug() << "new neuron";

        auto *neuron = storage_.get<Neuron>(neuron_id);
        auto *dendrite = storage_.get<Dendrite>(dendrite_id);
        auto *axon = storage_.get<Axon>(axon_id);

        auto *neuron_item = new NeuronItem(storage_, neuron);
        auto *dendrite_item = new DendriteItem(storage_, dendrite);
        auto *axon_item = new AxonItem(storage_, axon);

        scene_->addItem(neuron_item);
        scene_->addItem(dendrite_item);
        scene_->addItem(axon_item);

        items_.insert({neuron_id, neuron_item});
        items_.insert({dendrite_id, dendrite_item});
        items_.insert({axon_id, axon_item});
    }

    void onNewSynaps(const std::string &synaps_id) {
        auto *synaps = storage_.get<Synaps>(synaps_id);

        auto *synaps_item = new SynapsItem(storage_, synaps);

        scene_->addItem(synaps_item);

        items_.insert({synaps_id, synaps_item});
    }
};
