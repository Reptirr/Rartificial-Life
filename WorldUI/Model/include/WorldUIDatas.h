#pragma once
#include <EntityKind.h>
#include <QColor>
#include <QPolygonF>
#include <string>
#include <Uuid.h>

struct WorldObjectData {
    EntityKind entity_kind;
    Uuid entity_id;

    QPointF point;
    QColor color;
    QPolygonF poly;
    double angle{}; // degree

    virtual ~WorldObjectData() {}
    std::string getIdString() const {
        return entity_id.toString();
    }
};

struct AgentBodyData : WorldObjectData {
    double energy{};
};
