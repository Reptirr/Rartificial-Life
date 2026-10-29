#pragma once

#include <QPolygonF>
#include <box2d/box2d.h>

struct QtDebugDraw {
    QPainter *p = nullptr;
    float scale = 50.0f;   // пикселей на метр

    QPointF pt(float x, float y) const { return {x * scale, -y * scale}; }   // Y вверх -> вниз
    QPointF pt(b2Pos v) const { return pt(static_cast<float>(v.x), static_cast<float>(v.y)); }

    // локальная точка -> экран
    QPointF world(const b2WorldTransform &xf, b2Vec2 v) const {
        const float x = xf.q.c * v.x - xf.q.s * v.y + static_cast<float>(xf.p.x);
        const float y = xf.q.s * v.x + xf.q.c * v.y + static_cast<float>(xf.p.y);
        return pt(x, y);
    }

    static QColor col(b2HexColor c, int a = 255) { QColor q(static_cast<int>(c)); q.setAlpha(a); return q; }
    static QtDebugDraw &self(void *ctx) { return *static_cast<QtDebugDraw *>(ctx); }

    static void polygon(b2WorldTransform xf, const b2Vec2 *v, int n, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        QPolygonF poly;
        for (int i = 0; i < n; ++i) poly << d.world(xf, v[i]);
        d.p->setPen(col(c));
        d.p->setBrush(Qt::NoBrush);
        d.p->drawPolygon(poly);
    }

    static void solidPolygon(b2WorldTransform xf, const b2Vec2 *v, int n, float /*radius*/, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        QPolygonF poly;
        for (int i = 0; i < n; ++i) poly << d.world(xf, v[i]);
        d.p->setPen(col(c));
        d.p->setBrush(col(c, 90));
        d.p->drawPolygon(poly);
    }

    static void solidCircle(b2WorldTransform xf, b2Vec2 center, float r, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        const QPointF c0 = d.world(xf, center);
        d.p->setPen(col(c));
        d.p->setBrush(col(c, 90));
        d.p->drawEllipse(c0, r * d.scale, r * d.scale);
        d.p->drawLine(c0, d.world(xf, {center.x + r, center.y}));   // показывает поворот
    }

    static void solidCapsule(b2Pos a, b2Pos b, float r, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        const QPointF pa = d.pt(a), pb = d.pt(b);
        d.p->setPen(col(c));
        d.p->setBrush(col(c, 90));
        d.p->drawEllipse(pa, r * d.scale, r * d.scale);
        d.p->drawEllipse(pb, r * d.scale, r * d.scale);

        QPointF axis = pb - pa;
        const double len = std::hypot(axis.x(), axis.y());
        if (len > 1e-6) {
            axis /= len;
            const QPointF n(-axis.y() * r * d.scale, axis.x() * r * d.scale);
            d.p->setPen(Qt::NoPen);
            d.p->drawPolygon(QPolygonF{pa + n, pb + n, pb - n, pa - n});
        }
    }

    static void line(b2Pos a, b2Pos b, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        d.p->setPen(col(c));
        d.p->drawLine(d.pt(a), d.pt(b));
    }

    static void point(b2Pos v, float size, b2HexColor c, void *ctx) {
        auto &d = self(ctx);
        d.p->setPen(Qt::NoPen);
        d.p->setBrush(col(c));
        d.p->drawEllipse(d.pt(v), size / 2, size / 2);
    }
};