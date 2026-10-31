#pragma once

#include <EntityKind.h>
#include <qcolor.h>
#include <WorldDataCollector.h>
#include <box2d/id.h>

class WorldObject {
    WorldDataCollector &data_collector_;

    EntityKind entity_kind_{EntityKind::NONE};
    std::string this_id_;
    QColor color_;
    b2BodyId body_id_;


public:
    explicit WorldObject(const QColor &color, b2BodyId body_id, WorldDataCollector &data_collector)\
        : data_collector_(data_collector), color_(color), body_id_(body_id) {
    }


    void init(const std::string &entity_id, EntityKind entity_kind) {
        this_id_ = entity_id;
        entity_kind_ = entity_kind;

        data_collector_.onCreate(entity_id);
    }
    EntityKind getEntityKind() const {
        return entity_kind_;
    }
    std::string getThisId() const {
        return this_id_;
    }

    QColor getColor() const {
        return color_;
    }
    b2BodyId getBodyId() const {
        return body_id_;
    }
};
