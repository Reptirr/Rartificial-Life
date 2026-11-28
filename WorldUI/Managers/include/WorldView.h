#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWheelEvent>
#include <Uuid.h>
#include <World.h>
#include <WorldUIBatcher.h>
#include <WorldUIController.h>
#include <WorldUIMapper.h>
#include <WorldUIStorage.h>

class WorldView : public QGraphicsView {
    Q_OBJECT

    const WorldStorage *storage_{};
    WorldUIStorage ui_storage_;
    QGraphicsScene *scene_ = new QGraphicsScene(this);
    WorldUIController controller_{ui_storage_, scene_, this};
    WorldUIBatcher batcher_{ui_storage_, controller_};

    QTimer timer_;

protected:
    void wheelEvent(QWheelEvent *event) override {
        const int dy = event->angleDelta().y();
        if (dy == 0) {
            QGraphicsView::wheelEvent(event);
        }

        double factor = std::pow(1.0015, dy);

        scale(factor, factor);

        event->accept();
    }

public slots:
    void onAgentOpen(const Uuid &id) { // from WorldUIController
        emit agentOpen(id);
    }

    void onAgentTabOpen(Uuid id) {
        auto *obj = ui_storage_.getObject(id);

        obj->onPenEnable();
    }
    void onAgentTabClose(Uuid id) {
        auto *obj = ui_storage_.getObject(id);

        obj->onPenDisable();
    }

signals:
    void agentOpen(const Uuid &id);

public:
    explicit WorldView(QWidget *parent) : QGraphicsView(parent) {
        setScene(scene_);
        setTransformationAnchor(AnchorUnderMouse);
        setDragMode(ScrollHandDrag);

        // сглаживание
        setViewport(new QOpenGLWidget);
        setViewportUpdateMode(FullViewportUpdate);
        setRenderHint(QPainter::Antialiasing);

        setBackgroundBrush(QColorConstants::White);

        setSceneRect(-500*WorldUIConfig::pixelsPerMeter, -500*WorldUIConfig::pixelsPerMeter, 1000*WorldUIConfig::pixelsPerMeter, 1000*WorldUIConfig::pixelsPerMeter);

        connect(&timer_, &QTimer::timeout, [&] {
            batcher_.applyBatch();
        });
        timer_.start(16);
    }

    void connectWorld(World *world) {
        storage_ = world->getStorage();

        world->setCreateConsumer([&](const Uuid &id) { onCreate(id); });
        world->setUpdateConsumer([&](const Uuid &id) { onUpdate(id); });
        world->setRemoveConsumer([&](const Uuid &id) { onRemove(id); });
    }

    void onCreate(const Uuid &id) {
        batcher_.onCreate(
            id,
            WorldUIMapper::dataFromObjectId(id, storage_)
        );
    }
    void onUpdate(const Uuid &id) {
        batcher_.onUpdate(
            id,
            WorldUIMapper::dataFromObjectId(id, storage_)
        );
    }
    void onRemove(const Uuid &id) {
        batcher_.onRemove(id);
    }
};
