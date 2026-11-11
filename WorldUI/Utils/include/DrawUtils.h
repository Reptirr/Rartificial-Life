#pragma once

#include <cmath>
#include <QLineF>
#include <QPainter>
#include <QPolygonF>
#include <QStyleOptionGraphicsItem>

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

inline void drawScalableText(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             const QRectF &boundingRect,
                             const QString &symbol,
                             const qreal fontSize)   // размер шрифта в пикселях в координатах сцены
{
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    const qreal screenFontSize = fontSize * lod;     // размер шрифта в пикселях на экране

    constexpr qreal kMinReadableFontSize = 6.0;      // порог в пикселях, подбери глазами
    if (screenFontSize < kMinReadableFontSize)
        return;

    painter->save();

    const QPointF center = boundingRect.center();

    // убираем масштаб на время рисования текста
    painter->translate(center);
    painter->scale(1.0 / lod, 1.0 / lod);
    painter->translate(-center);

    QFont font;   // шрифт по умолчанию, а не остаток от прошлой отрисовки
    font.setPixelSize(qMax(1, qRound(screenFontSize)));
    font.setHintingPreference(QFont::PreferNoHinting);
    painter->setFont(font);
    painter->setPen(Qt::black);

    // прямоугольник в экранных единицах, чтобы перенос слов работал правильно
    QRectF textRect{0, 0, boundingRect.width() * lod, boundingRect.height() * lod};
    textRect.moveCenter(center);

    painter->setClipRect(textRect);   // текст не вылезает за пределы элемента
    painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, symbol);

    painter->restore();
}