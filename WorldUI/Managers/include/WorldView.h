#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QWheelEvent>
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
    void drawBackground(QPainter *painter, const QRectF &rect) override {

    }

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
    void onAgentOpen(std::string id) {
        emit agentOpen(id);
    }

signals:
    void agentOpen(std::string id);

public:
    explicit WorldView(QWidget *parent) : QGraphicsView(parent) {
        setScene(scene_);
        setRenderHints(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setDragMode(ScrollHandDrag);
        setBackgroundBrush(QColorConstants::DarkGreen);
        setViewportUpdateMode(FullViewportUpdate);

        setSceneRect(-500*WorldUIConfig::pixelsPerMeter, -500*WorldUIConfig::pixelsPerMeter, 1000*WorldUIConfig::pixelsPerMeter, 1000*WorldUIConfig::pixelsPerMeter);

        connect(&timer_, &QTimer::timeout, [&] {
            batcher_.applyBatch();
        });
        timer_.start(16);
    }

    void connectWorld(World *world) {
        storage_ = world->getStorage();

        world->setCreateConsumer([&](const std::string &id) { onCreate(id); });
        world->setUpdateConsumer([&](const std::string &id) { onUpdate(id); });
        world->setRemoveConsumer([&](const std::string &id) { onRemove(id); });
    }

    void onCreate(const std::string &id) {
        batcher_.onCreate(
            id,
            WorldUIMapper::dataFromObjectId(id, storage_)
        );
    }
    void onUpdate(const std::string &id) {
        batcher_.onUpdate(
            id,
            WorldUIMapper::dataFromObjectId(id, storage_)
        );
    }
    void onRemove(const std::string &id) {
        batcher_.onRemove(id);
    }
};
