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


std::vector<QColor> SenseGetter::getVision(QPointF eye, const double azimuth, const double fov,
                                           const int rays, const float maxDist,
                                           const QColor default_color, const std::string &exclude_obj) const {
    std::vector<QColor> res;
    res.reserve(rays);

    b2Pos from = {.x = static_cast<float>(eye.x()), .y = static_cast<float>(eye.y())};

    const double ray_angle = fov / rays;
    double angle = azimuth - fov / 2 + ray_angle / 2; // центр сектора, а не его край

    for (int i = 0; i < rays; i++) {
        // azimuth -> vec
        b2Vec2 dir = { .x = static_cast<float>(std::sin(angle)), .y = static_cast<float>(std::cos(angle)) };
        b2Vec2 translation = {.x = dir.x * maxDist, .y = dir.y * maxDist};

        auto hit = b2World_CastRayClosest(storage_.getWorldId(), from, translation, b2DefaultQueryFilter());

        WorldObject *obj{};

        if (hit.hit) {
            auto body = b2Shape_GetBody(hit.shapeId);
            obj = static_cast<WorldObject *>(b2Body_GetUserData(body));
            my_assert(obj != nullptr);
        }

        if (obj) res.push_back(obj->getColor());
        else res.push_back(default_color);

        angle += ray_angle;
    }

    return res;
}