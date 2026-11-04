#pragma once
#include <DrawUtils.h>
#include <qpainter.h>
#include <WorldObjectItem.h>

class AgentBodyItem : public WorldObjectItem {
    AgentBodyData *my_data_{};

    static QPointF moveByAzimuth(QPointF p, double azimuth, double length) {
        return { p.x() + length * std::sin(azimuth),
            p.y() + length * std::cos(azimuth) };
    }

protected:
    void onSetData() override {
        my_data_ = dynamic_cast<AgentBodyData *>(data_);
    }

    void onUIUpdate() override {
        update();
    }

public:
    explicit AgentBodyItem(const WorldObjectData *data)
        : WorldObjectItem(data) {
    }

    QRectF boundingRect() const override {
        auto res = WorldObjectItem::boundingRect();
        res.adjust(0, 0, WorldUIConfig::agent_angle_length + WorldUIConfig::agent_angle_head_size, 0);

        return res;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // рисуем стрелку поворота
        const QPen pen{WorldUIConfig::agent_angle_color, WorldUIConfig::agent_angle_w, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin};
        const QBrush brush{WorldUIConfig::agent_angle_color};
        painter->setPen(pen);
        painter->setBrush(brush);


        drawArrow(
            *painter,
            {},
            moveByAzimuth({}, my_data_->azimuth, WorldUIConfig::agent_angle_length),
            WorldUIConfig::agent_angle_head_size
        );

        WorldObjectItem::paint(painter, option, widget);
    }
};
