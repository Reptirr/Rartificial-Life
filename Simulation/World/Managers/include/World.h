#pragma once
#include <WorldController.h>
#include <WorldDataCollector.h>
#include <WorldStorage.h>
#include <box2d/box2d.h>

class World {
    std::unique_ptr<WorldStorage> storage_;
    std::unique_ptr<WorldController> controller_;
    WorldDataCollector data_collector_;

public:
    World() {
        auto wd = b2DefaultWorldDef();
        wd.gravity = {};
        auto world_id = b2CreateWorld(&wd);

        storage_ = std::make_unique<WorldStorage>(world_id);
        controller_ = std::make_unique<WorldController>(*storage_, data_collector_);
    }

    void tick(const double dt) {
        b2World_Step(storage_->getWorldId(), dt, 2);
        auto events = b2World_GetBodyEvents(storage_->getWorldId());
        for (int i = 0; i < events.moveCount; i++) {
            auto event = events.moveEvents[i];
            auto *obj = static_cast<WorldObject *>(b2Body_GetUserData(event.bodyId));
            my_assert(obj != nullptr);

            obj->onUpdate();
        }
    }

    std::string addAgentBody(QPointF pos, AgentBodyConfig conf) const {
        return controller_->addAgentBody(pos, conf);
    }

    std::string addObstacle(QPointF pos, QRectF rect, QColor color) const {
        return controller_->addObstacle(pos, rect, color);
    }

    const WorldStorage *getStorage() const {
        return storage_.get();
    }

    // data collector
    void setCreateConsumer(const StringConsumer &cons) {
        data_collector_.setCreateConsumer(cons);
    }
    void setUpdateConsumer(const StringConsumer &cons) {
        data_collector_.setUpdateConsumer(cons);
    }
};
