#pragma once

#include <QGraphicsPolygonItem>
#include <qpen.h>
#include <WorldUIConfig.h>
#include <WorldUIDatas.h>

class WorldObjectItem : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT

protected:
    WorldObjectData *data_{}; // удаляет сам WorldObjectItem

    virtual void onSetData() {}
    virtual void onUIUpdate() {}

    bool press{};

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override {
        emit mouseDoubleClick(data_->entity_id);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        press = true;
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        if (press) {
            mouseClick(data_->entity_id);
            press = false;
        }
    }

signals:
    void mouseDoubleClick(std::string id);
    void mouseClick(std::string id);

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

     void UIUpdate() {
        if (pos() != data_->point)
            setPos(data_->point);
        if (polygon() != data_->poly)
            setPolygon(data_->poly);
        QPen p{data_->color, WorldUIConfig::world_obj_pen_w};
        p.setJoinStyle(Qt::MiterJoin);
        if (pen() != p) setPen(p);
        setBrush(data_->color);
        if (data_->angle != rotation())
            setRotation(data_->angle);

        onUIUpdate();
    }

    ~WorldObjectItem() override {
        delete data_;
    }
};
