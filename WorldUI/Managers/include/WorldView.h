#pragma once

#include <QtDebugDraw.h>
#include <box2d/box2d.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>
#include <cmath>

class WorldView : public QGraphicsView {
    b2WorldId world_id_;
    QGraphicsScene scene_;

    static constexpr double kPpm = 1.0;  // пикселей на метр при старте

public:
    explicit WorldView(const b2WorldId id, QWidget *parent = nullptr)
        : QGraphicsView(parent), world_id_(id) {
        setScene(&scene_);
        scene_.setSceneRect(-1e6, -1e6, 2e6, 2e6);  // «бесконечная» плоскость

        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag);  // тянем ЛКМ, чтобы двигать вид
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        scale(kPpm, -kPpm);  // минус: у Box2D Y вверх, у Qt вниз
        centerOn(0, 0);

        auto *timer = new QTimer(this);  // перерисовка, пока идёт симуляция
        connect(timer, &QTimer::timeout, viewport(), qOverload<>(&QWidget::update));
        timer->start(16);
    }

protected:
    void wheelEvent(QWheelEvent *e) override {
        const double f = std::pow(1.15, e->angleDelta().y() / 120.0);
        const double px = transform().m11();
        if (px * f < 2.0 || px * f > 2000.0) return;  // ограничение зума
        scale(f, f);
    }

    void drawBackground(QPainter *p, const QRectF &r) override {
        const double px = transform().m11();  // пикселей на метр сейчас

        // шаг сетки: при отдалении укрупняем, чтобы линии не слипались
        double step = 1.0;
        while (step * px < 12.0) step *= 10.0;

        p->fillRect(r, QColor(30, 30, 34));

        QPen minor(QColor(50, 50, 56), 0);   // ширина 0 = всегда 1 пиксель
        QPen major(QColor(80, 80, 90), 0);

        const long long x0 = std::floor(r.left() / step), x1 = std::ceil(r.right() / step);
        const long long y0 = std::floor(r.top() / step),  y1 = std::ceil(r.bottom() / step);

        for (long long i = x0; i <= x1; ++i) {
            p->setPen(i % 10 == 0 ? major : minor);
            p->drawLine(QPointF(i * step, r.top()), QPointF(i * step, r.bottom()));
        }
        for (long long j = y0; j <= y1; ++j) {
            p->setPen(j % 10 == 0 ? major : minor);
            p->drawLine(QPointF(r.left(), j * step), QPointF(r.right(), j * step));
        }

        // оси
        p->setPen(QPen(QColor(220, 70, 70), 0));
        p->drawLine(QPointF(r.left(), 0), QPointF(r.right(), 0));
        p->setPen(QPen(QColor(70, 200, 90), 0));
        p->drawLine(QPointF(0, r.top()), QPointF(0, r.bottom()));

        // подписи рисуем в пикселях, иначе текст будет перевёрнут и масштабирован
        p->save();
        p->resetTransform();
        p->setPen(QColor(150, 150, 160));
        for (long long i = x0; i <= x1; ++i) {
            if (i % 10 != 0) continue;
            QPoint s = mapFromScene(QPointF(i * step, 0));
            p->drawText(s + QPoint(3, 13), QString::number(i * step));
        }
        for (long long j = y0; j <= y1; ++j) {
            if (j % 10 != 0 || j == 0) continue;
            QPoint s = mapFromScene(QPointF(0, j * step));
            p->drawText(s + QPoint(4, -3), QString::number(j * step));
        }
        p->restore();
    }

    void drawForeground(QPainter *p, const QRectF &) override {
        static QtDebugDraw ctx;
        ctx.p = p;

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
};