#pragma once
#include <WorldStorage.h>
#include <box2d/box2d.h>

class MoveController {
    WorldStorage &storage_;

public:
    explicit MoveController(WorldStorage &storage)
        : storage_(storage) {
    }

    void setVel(const std::string &id, const b2Vec2 vec) const {
        const auto obj = storage_.getObject(id);
        const auto body = obj->getBodyId();

        b2Body_ApplyForceToCenter(body, vec, true);
    }
};
