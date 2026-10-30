#pragma once
#include <WorldStorage.h>
#include <box2d/box2d.h>

class MoveController {
    WorldStorage &storage_;

public:
    explicit MoveController(WorldStorage &storage)
        : storage_(storage) {
    }

    void setVel(const std::string &id, const double v) const {
        const auto obj = storage_.getObject(id);
        const auto body = obj->getBodyId();

        auto [c, s] = b2Body_GetRotation(body);
        const b2Vec2 dir = {.x = s, .y = c};
        const auto vec = dir * v;

        b2Body_ApplyForceToCenter(body, vec, true);
    }
};
