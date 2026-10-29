#pragma once
#include <qcolor.h>
#include <qpolygon.h>
#include <box2d/id.h>

class WorldObject {
    std::string entity_id_;
    QColor color_;
    b2BodyId body_id_;


public:
    explicit WorldObject(const QColor &color, b2BodyId body_id) : color_(color), body_id_(body_id) {
    }


    void init(const std::string &entity_id) {
        entity_id_ = entity_id;
    }
    std::string getEntity() const {
        return entity_id_;
    }

    QColor getColor() const {
        return color_;
    }
    b2BodyId getBodyId() const {
        return body_id_;
    }
};
