#pragma once
#include <WorldObject.h>
#include <WorldUIConfig.h>
#include <WorldUIDatas.h>
#include <box2d/box2d.h>

// точки дуги вокруг центра c, от угла a0 до a1
inline void appendArc(QPolygonF& out, b2Vec2 c, float r, double a0, double a1) {
    for (int i = 0; i <= WorldUIConfig::kArcSteps; ++i) {
        double a = a0 + (a1 - a0) * i / WorldUIConfig::kArcSteps;
        out << QPointF(c.x + r * std::cos(a), c.y + r * std::sin(a));
    }
}


// точки в локальных координатах тела
inline QPolygonF convertShapeToQPolygonF(const b2ShapeId shape) {
    using std::numbers::pi;
    QPolygonF res;

    switch (b2Shape_GetType(shape)) {
        case b2_circleShape: {
            b2Circle c = b2Shape_GetCircle(shape);
            appendArc(res, c.center, c.radius, 0.0, 2.0 * pi);
            res.removeLast();   // последняя точка совпадает с первой
            break;
        }
        case b2_capsuleShape: {
            b2Capsule c = b2Shape_GetCapsule(shape);
            double a = std::atan2(c.center2.y - c.center1.y, c.center2.x - c.center1.x);
            appendArc(res, c.center2, c.radius, a - pi / 2, a + pi / 2);      // вокруг конца 2
            appendArc(res, c.center1, c.radius, a + pi / 2, a + 3 * pi / 2);  // вокруг конца 1
            break;
        }
        case b2_segmentShape: {
            b2Segment s = b2Shape_GetSegment(shape);
            res << QPointF(s.point1.x, s.point1.y) << QPointF(s.point2.x, s.point2.y);
            break;
        }
        case b2_polygonShape: {
            b2Polygon p = b2Shape_GetPolygon(shape);
            for (int i = 0; i < p.count; ++i)
                res << QPointF(p.vertices[i].x, p.vertices[i].y);
            break;
        }
        case b2_chainSegmentShape: {
            const b2ChainSegment cs = b2Shape_GetChainSegment(shape);
            res << QPointF(cs.segment.point1.x, cs.segment.point1.y)
                << QPointF(cs.segment.point2.x, cs.segment.point2.y);
            break;
        }
        case b2_shapeTypeCount:
            break;
    }
    return res;
}


class WorldUIMapper {
public:
    static WorldObjectData convertWorldObject(const WorldObject *obj) {
        WorldObjectData data{};

        data.entity_kind = obj->getEntityKind();
        data.entity_id = obj->getThisId();
        data.color = obj->getColor();

        auto pos = b2Body_GetPosition(obj->getBodyId());
        data.point = QPointF{pos.x, pos.y};

        b2ShapeId shape;
        b2Body_GetShapes(obj->getBodyId(), &shape, 1);
        data.poly = convertShapeToQPolygonF(shape);

        return data;
    }
};
