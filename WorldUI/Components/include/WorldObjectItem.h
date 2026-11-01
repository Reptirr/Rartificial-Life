#pragma once

#include <QGraphicsPolygonItem>
#include <qpen.h>
#include <WorldUIConfig.h>
#include <WorldUIDatas.h>

class WorldObjectItem : public QGraphicsPolygonItem {
protected:
    WorldObjectData *data_{}; // удаляет сам WorldObjectItem

    virtual void onSetData() {}
    virtual void onUIUpdate() {}

public:
    explicit WorldObjectItem(const WorldObjectData *data) : QGraphicsPolygonItem(data->poly) {
        setPos(data->point);
        QPen pen{data->color, WorldUIConfig::world_obj_pen_w};
        pen.setJoinStyle(Qt::MiterJoin);
        setPen(pen);
        setBrush(data->color);
    }

    void setData(WorldObjectData *data) {
        delete data_;
        data_ = data;
        onSetData();
    }

     virtual void UIUpdate() {
        if (pos() != data_->point) setPos(data_->point);
        if (polygon() != data_->poly) setPolygon(data_->poly);
        QPen p{data_->color, WorldUIConfig::world_obj_pen_w};
        p.setJoinStyle(Qt::MiterJoin);
        if (pen() != p) setPen(p);
        setBrush(data_->color);

        onUIUpdate();
    }

    ~WorldObjectItem() override {
        delete data_;
    }
};
