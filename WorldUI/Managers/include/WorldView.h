#pragma once
#include <WorldUIBatcher.h>
#include <WorldUIController.h>
#include <WorldUIMapper.h>
#include <WorldUIStorage.h>

class WorldView : public QGraphicsView {
    const WorldStorage *storage_{};
    WorldUIStorage ui_storage_;
    QGraphicsScene *scene_ = new QGraphicsScene(this);
    WorldUIController controller_{ui_storage_, scene_};
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

public:
    explicit WorldView(QWidget *parent) : QGraphicsView(parent) {
        setScene(scene_);
        setRenderHints(QPainter::Antialiasing);
        setTransformationAnchor(AnchorUnderMouse);
        setDragMode(ScrollHandDrag);
        setBackgroundBrush(QColorConstants::DarkGreen);

        setSceneRect(-500, -500, 1000, 1000);

        connect(&timer_, &QTimer::timeout, [&] {
            batcher_.applyBatch();
        });
        timer_.start(16);
    }

    void connectWorld(World *world) {
        storage_ = world->getStorage();

        world->setCreateConsumer([&](const std::string &id) { onCreate(id); });
        world->setUpdateConsumer([&](const std::string &id) { onUpdate(id); });
    }

    void onCreate(const std::string &id) {
        const auto *obj = storage_->getObject(id);

        batcher_.onCreate(
            id,
            WorldUIMapper::convertWorldObject(obj)
        );
    }
    void onUpdate(const std::string &id) {
        const auto *obj = storage_->getObject(id);

        batcher_.onUpdate(
            id,
            WorldUIMapper::convertWorldObject(obj)
        );
    }
};
