#pragma once
#include <QColor>
#include <QPolygonF>
#include <string>

struct WorldObjectData {
    EntityKind entity_kind;
    std::string entity_id;

    QPointF point;
    QColor color;
    QPolygonF poly;
};
