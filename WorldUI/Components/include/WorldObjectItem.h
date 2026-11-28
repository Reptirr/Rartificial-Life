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

        QGraphicsPolygonItem::mouseDoubleClickEvent(event);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        press = true;

        QGraphicsPolygonItem::mousePressEvent(event);
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override {
        if (press) {
            emit mouseClick(data_->entity_id);
            press = false;
        }

        QGraphicsPolygonItem::mouseReleaseEvent(event);
    }

signals:
    void mouseDoubleClick(const Uuid &id);
    void mouseClick(const Uuid &id);

public: // slots
    void onPenEnable() {
        setPen(QPen{WorldUIConfig::world_obj_light_pen_color, WorldUIConfig::world_obj_light_pen_w});

        UIUpdate();
    }
    void onPenDisable() {
        setPen({QColorConstants::Green, WorldUIConfig::world_obj_pen_w});

        UIUpdate();
    }

public:
    explicit WorldObjectItem(const WorldObjectData *data) : QGraphicsPolygonItem(data->poly) {
        setPos(data->point);
        QPen pen{data->color, WorldUIConfig::world_obj_pen_w};
        pen.setJoinStyle(Qt::MiterJoin);
        setPen(pen);
        setBrush(data->color);
    }

    QRectF boundingRect() const override {
        return QGraphicsPolygonItem::boundingRect().adjusted(-0.5*WorldUIConfig::world_obj_light_pen_w, -0.5*WorldUIConfig::world_obj_light_pen_w, 0.5*WorldUIConfig::world_obj_light_pen_w, 0.5*WorldUIConfig::world_obj_light_pen_w);
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
        setBrush(data_->color);
        if (data_->angle != rotation())
            setRotation(data_->angle);

        onUIUpdate();
    }

    ~WorldObjectItem() override {
        delete data_;
    }
};
