#pragma once
#include <WorldController.h>
#include <WorldDataCollector.h>
#include <WorldStorage.h>
#include <box2d/box2d.h>

struct AgentBodyConfig;

class World {
    std::unique_ptr<WorldStorage> storage_;
    std::unique_ptr<WorldController> controller_;
    std::unique_ptr<WorldExecutor> executor_;
    WorldDataCollector data_collector_;

    double dt_;

public:
    World(double dt) : dt_(dt) {
        auto wd = b2DefaultWorldDef();
        wd.gravity = {};
        wd.enableContinuous = true;
        auto world_id = b2CreateWorld(&wd);

        storage_ = std::make_unique<WorldStorage>(world_id);
        executor_ = std::make_unique<WorldExecutor>();
        controller_ = std::make_unique<WorldController>(*storage_, data_collector_, executor_.get(), dt);
        executor_->init(storage_.get(), controller_.get());
    }

    void init(SimulationController *controller) {
        controller_->init(controller);
    }

    void tick() {
        b2World_Step(storage_->getWorldId(), dt_, 4);

        auto body_events = b2World_GetBodyEvents(storage_->getWorldId());
        for (int i = 0; i < body_events.moveCount; i++) {
            auto event = body_events.moveEvents[i];
            auto *obj = static_cast<WorldObject *>(b2Body_GetUserData(event.bodyId));
            my_assert(obj != nullptr);

            obj->onUpdate();
        }

        auto contact_events = b2World_GetContactEvents(storage_->getWorldId());
        for (int i = 0; i < contact_events.beginCount; i++) {
            auto event = contact_events.beginEvents[i];

            auto body1 = b2Shape_GetBody(event.shapeIdA);
            auto body2 = b2Shape_GetBody(event.shapeIdB);

            auto *obj1 = static_cast<WorldObject *>(b2Body_GetUserData(body1));
            auto *obj2 = static_cast<WorldObject *>(b2Body_GetUserData(body2));

            auto *entity1 = storage_->getEntity(obj1->getThisId());
            auto *entity2 = storage_->getEntity(obj2->getThisId());

            if (entity1 && entity2) {
                entity1->onCollision(entity2);
                entity2->onCollision(entity1);
            }
        }

        executor_->apply(); // moves, births, deaths, removes
    }

    std::string addAgentBody(const AgentBodyConfig &conf) const {
        return controller_->addAgentBody(conf);
    }

    std::string addObstacle(QPointF pos, QRectF rect, QColor color) const {
        return controller_->addObstacle(pos, rect, color);
    }
    std::string addFood(QPointF pos) const {
        return controller_->addFood(pos);
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
    void setRemoveConsumer(const StringConsumer &cons) {
        data_collector_.setRemoveConsumer(cons);
    }
};
