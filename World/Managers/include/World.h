#pragma once
#include <WorldController.h>
#include <WorldStorage.h>
#include <box2d/box2d.h>

class World {
    std::unique_ptr<WorldStorage> storage_;
    std::unique_ptr<WorldController> controller_;


public:
    World() {
        auto wd = b2DefaultWorldDef();
        wd.gravity = {};
        auto world_id = b2CreateWorld(&wd);

        storage_ = std::make_unique<WorldStorage>(world_id);
        controller_ = std::make_unique<WorldController>(*storage_);

    }

    std::string addAgentBody(QPointF pos) const {
        return controller_->addAgentBody(pos);
    }

    std::string addObstacle(QPointF pos) const {
        return controller_->addObstacle(pos);
    }

    const WorldStorage *getStorage() const {
        return storage_.get();
    }
};
