#pragma once

#include <QGraphicsEllipseItem>
#include <qpen.h>
#include <UIComponent.h>
#include <UIConfig.h>

class NeuronItem : public QGraphicsEllipseItem, public UIComponent {
    Neuron *neuron_;

public:
    NeuronItem(Storage &storage, Neuron *neuron) : QGraphicsEllipseItem(-UIConfig::neuron_r, -UIConfig::neuron_r,
                                                        UIConfig::neuron_r * 2, UIConfig::neuron_r * 2), UIComponent(storage) {
        neuron_ = neuron;
        qDebug() << "new neuron at " << toQPointF(neuron_->point());
        setPos(toQPointF(neuron_->point()));

        setZValue(UIConfig::neuron_z);

        setPen(QPen{Qt::black, 0.002});
        setBrush(QBrush{QColorConstants::DarkCyan});
        update();
    }
};
