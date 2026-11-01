#pragma once

#include <QPainter>
#include <QPolygonF>
#include <QLineF>
#include <cmath>

inline void drawArrow(QPainter& p, const QPointF& from, const QPointF& to,
                      qreal headSize = 10.0)
{
    QLineF line(from, to);
    if (line.length() < 1e-6) return;

    // линия (чуть укорачиваем, чтобы она не торчала сквозь наконечник)
    QLineF shaft = line;
    shaft.setLength(line.length() - headSize * 0.8);
    p.drawLine(shaft);

    // угол линии (в Qt ось Y смотрит вниз, поэтому минус)
    qreal angle = std::atan2(-line.dy(), line.dx());

    QPointF p1 = to - QPointF(std::cos(angle + M_PI / 6) * headSize,
                              -std::sin(angle + M_PI / 6) * headSize);
    QPointF p2 = to - QPointF(std::cos(angle - M_PI / 6) * headSize,
                              -std::sin(angle - M_PI / 6) * headSize);

    p.setBrush(p.pen().color());          // залить наконечник тем же цветом
    p.drawPolygon(QPolygonF() << to << p1 << p2);
}