#pragma once
#include <AgentBody.h>
#include <WorldObject.h>
#include <WorldUIConfig.h>
#include <WorldUIDatas.h>
#include <box2d/box2d.h>

inline QPointF toScreen(b2Vec2 p) {
    return {
        p.x * WorldUIConfig::pixelsPerMeter,
        p.y * WorldUIConfig::pixelsPerMeter
    };
}

// точки дуги вокруг центра c, от угла a0 до a1
inline void appendArc(QPolygonF& out, b2Vec2 c, float r, double a0, double a1) {
    for (int i = 0; i <= WorldUIConfig::kArcSteps; ++i) {
        double a = a0 + (a1 - a0) * i / WorldUIConfig::kArcSteps;

        b2Vec2 p{
            static_cast<float>(c.x + r * std::cos(a)),
            static_cast<float>(c.y + r * std::sin(a))
        };

        out << toScreen(p);
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
            res << toScreen(s.point1) << toScreen(s.point1);
            break;
        }
        case b2_polygonShape: {
            b2Polygon p = b2Shape_GetPolygon(shape);
            for (int i = 0; i < p.count; ++i)
                res << toScreen(p.vertices[i]);
            break;
        }
        case b2_chainSegmentShape: {
            const b2ChainSegment cs = b2Shape_GetChainSegment(shape);
            res << toScreen(cs.segment.point1)
                << toScreen(cs.segment.point2);
            break;
        }
        case b2_shapeTypeCount:
            break;
    }
    return res;
}


class WorldUIMapper {
    static void fillData(WorldObjectData *data, const WorldObject *obj) {
        data->entity_kind = obj->getEntityKind();
        data->entity_id = obj->getThisId();
        data->color = obj->getColor();

        auto pos = b2Body_GetPosition(obj->getBodyId());
        data->point = toScreen(pos);

        b2ShapeId shape;
        b2Body_GetShapes(obj->getBodyId(), &shape, 1);
        data->poly = convertShapeToQPolygonF(shape);

        auto rot = b2Body_GetRotation(obj->getBodyId());
        auto rad = b2Rot_GetAngle(rot);
        data->angle = qRadiansToDegrees(rad);
    }

public:
    static WorldObjectData *convertWorldObject(const WorldObject *obj) {
        auto *data = new WorldObjectData;

        fillData(data, obj);

        return data;
    }

    static WorldObjectData *convertAgentBody(const WorldObject *obj, const WorldStorage *storage) {
        const auto *agentbody = storage->getAgentBody(obj->getThisId());
        auto *data = new AgentBodyData;

        // world obj data
        fillData(data, obj);

        data->energy = agentbody->getEnergy();

        return data;
    }
    static WorldObjectData *convertFood(const WorldObject *obj, const WorldStorage *storage) {
        const auto *food = storage->getFood(obj->getThisId());
        auto *data = new AgentBodyData;

        // world obj data
        fillData(data, obj);

        return data;
    }

    static WorldObjectData *dataFromObjectId(const std::string &id, const WorldStorage *storage) {
        const auto obj = storage->getObject(id);

        auto *data = new WorldObjectData;

        switch (obj->getEntityKind()) {
            case EntityKind::NONE:
                data = convertWorldObject(obj);
                break;
            case EntityKind::AGENT_BODY:
                data = convertAgentBody(obj, storage);
                break;
            case EntityKind::FOOD:
                data = convertFood(obj, storage);
                break;

            default: my_assert(false);
        }

        return data;
    }
};
