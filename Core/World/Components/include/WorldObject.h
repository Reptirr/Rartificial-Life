#pragma once

#include <EntityKind.h>
#include <qcolor.h>
#include <WorldDataCollector.h>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <qdebug.h>
#include <Uuid.h>

class WorldObject {
    WorldDataCollector &data_collector_;

    EntityKind entity_kind_{EntityKind::NONE};
    Uuid this_id_;
    QColor color_;
    b2BodyId body_id_;


public:
    explicit WorldObject(const QColor &color, b2BodyId body_id, WorldDataCollector &data_collector)\
        : data_collector_(data_collector), color_(color), body_id_(body_id) {
    }

    void onUpdate() {
        data_collector_.onUpdate(this_id_);
    }

    void init(const Uuid &entity_id, EntityKind entity_kind) {
        this_id_ = entity_id;
        entity_kind_ = entity_kind;

        data_collector_.onCreate(entity_id);
    }
    EntityKind getEntityKind() const {
        return entity_kind_;
    }
    Uuid getThisId() const {
        return this_id_;
    }
    std::string getThisIdString() const {
        return this_id_.toString();
    }

    QColor getColor() const {
        return color_;
    }
    b2BodyId getBodyId() const {
        return body_id_;
    }
};
