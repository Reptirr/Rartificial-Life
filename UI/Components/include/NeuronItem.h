#pragma once

#include <QGraphicsEllipseItem>
#include <qpen.h>
#include <UIComponent.h>
#include <UIConfig.h>

class NeuronItem : public QGraphicsEllipseItem, public UIComponent {
    NeuronData data_;

public:
    NeuronItem(UIStorage &storage, const NeuronData &data) : QGraphicsEllipseItem(-UIConfig::neuron_r, -UIConfig::neuron_r,
                                                        UIConfig::neuron_r * 2, UIConfig::neuron_r * 2), UIComponent(storage) {
        data_ = data;
        qDebug() << "new neuron at " << data_.pos;
        setPos(data_.pos);

        setZValue(UIConfig::neuron_z);

        setPen(QPen{Qt::black, 0.002});
        setBrush(QBrush{QColorConstants::DarkCyan});
        update();
    }
};
