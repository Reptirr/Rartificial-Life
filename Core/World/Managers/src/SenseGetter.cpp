#include <my_assert.h>
#include <qcolor.h>
#include <QPolygonF>
#include <SenseGetter.h>
#include <box2d/box2d.h>


std::vector<QLineF> SenseGetter::polygonEdges(const QPolygonF &poly) const {
    std::vector<QLineF> edges;
    int n = poly.size();
    if (n < 2) return edges;

    // QPolygonF может быть уже "замкнут" (последняя точка == первая)
    if (poly.isClosed()) --n;

    edges.reserve(n);
    for (int i = 0; i < n; ++i)
        edges.emplace_back(poly[i], poly[(i + 1) % n]);

    return edges;
}

QPointF SenseGetter::pointByAzimuth(const QPointF from, const double azimuth, const double dist) {
    return {
        from.x() + std::cos(azimuth) * dist,
        from.y() + std::sin(azimuth) * dist
    };
}

QLineF SenseGetter::rayByAzimuth(QPointF from, const double azimuth, const double dist) {
    return {
        from,
        pointByAzimuth(from, azimuth, dist)
    };
}


std::vector<QColor> SenseGetter::getVision(
    std::string id,
    const double fov,
    const int rays,
    const float maxDist,
    const QColor default_color,
    const std::string &exclude_obj
) const {
    std::vector<QColor> res;

    if (rays <= 0 || fov <= 0.0 || maxDist <= 0.0f)
        return res;

    res.reserve(rays);

    auto *object = storage_.getObject(id);
    const auto bodyId = object->getBodyId();

    const auto from = b2Body_GetPosition(bodyId);
    const auto rotation = b2Body_GetRotation(bodyId);

    const auto forward = b2Rot_GetXAxis(rotation);
    const auto side = b2Rot_GetYAxis(rotation);

    const double ray_angle = fov / rays;

    for (int i = 0; i < rays; i++) {
        const float angle = static_cast<float>(
            -fov / 2.0 + ray_angle * (i + 0.5)
        );

        const float c = std::cos(angle);
        const float s = std::sin(angle);

        // направление луча в мировых координатах
        b2Vec2 dir = {
            .x = forward.x * c + side.x * s,
            .y = forward.y * c + side.y * s
        };

        b2Vec2 translation = {
            .x = dir.x * maxDist,
            .y = dir.y * maxDist
        };

        auto hit = b2World_CastRayClosest(
            storage_.getWorldId(),
            from,
            translation,
            b2DefaultQueryFilter()
        );

        WorldObject *obj{};

        if (hit.hit) {
            auto body = b2Shape_GetBody(hit.shapeId);
            obj = static_cast<WorldObject *>(
                b2Body_GetUserData(body)
            );
            my_assert(obj != nullptr);
        }

        res.push_back(obj ? obj->getColor() : default_color);
    }

    return res;
}