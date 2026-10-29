#pragma once

#include <QtDebugDraw.h>
#include <box2d/id.h>

class WorldView : public QWidget {
    b2WorldId world_id_;

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);                        // this = виджет, на котором рисуем
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(width() / 2, height() / 2);

        static QtDebugDraw ctx;
        ctx.p = &painter;

        b2DebugDraw dd = b2DefaultDebugDraw();
        dd.context = &ctx;
        dd.DrawPolygonFcn      = QtDebugDraw::polygon;
        dd.DrawSolidPolygonFcn = QtDebugDraw::solidPolygon;
        dd.DrawSolidCircleFcn  = QtDebugDraw::solidCircle;
        dd.DrawSolidCapsuleFcn = QtDebugDraw::solidCapsule;
        dd.DrawLineFcn         = QtDebugDraw::line;
        dd.DrawPointFcn        = QtDebugDraw::point;
        dd.drawShapes = true;

        b2World_Draw(world_id_, &dd);
    }

public:
    explicit WorldView(const b2WorldId id) : world_id_(id) {}

};
