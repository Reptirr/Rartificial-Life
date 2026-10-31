#pragma once

#include <QGraphicsPolygonItem>
#include <qpen.h>
#include <WorldUIConfig.h>
#include <WorldUIDatas.h>

class WorldObjectItem : public QGraphicsPolygonItem {
protected:
    WorldObjectData data_;

public:
    explicit WorldObjectItem(WorldObjectData data) : QGraphicsPolygonItem(data.poly), data_(data) {
        setPos(data_.point);
        QPen pen{data_.color, WorldUIConfig::world_obj_pen_w};
        pen.setJoinStyle(Qt::MiterJoin);
        setPen(pen);
        setBrush(data_.color);
    }

    void setData(const WorldObjectData &data) {
        data_ = data;
    }

     virtual void UIUpdate() {
        if (pos() != data_.point) setPos(data_.point);
        if (polygon() != data_.poly) setPolygon(data_.poly);
        QPen p{data_.color, WorldUIConfig::world_obj_pen_w};
        p.setJoinStyle(Qt::MiterJoin);
        if (pen() != p) setPen(p);
        setBrush(data_.color);
    }
};
