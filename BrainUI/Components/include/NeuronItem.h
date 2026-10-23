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

        setPen(QPen{Qt::black, 2});
        setBrush(QBrush{QColorConstants::DarkCyan});
        update();
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<NeuronData *>(d)) {
            data_ = *data;
        }
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QGraphicsEllipseItem::paint(painter, option, widget);

        // input or output text
        QPen pen{QColorConstants::Black};
        painter->setPen(pen);

        QString symbol{};

        auto font = painter->font();
        font.setPointSizeF(UIConfig::neuron_type_font_size);
        painter->setFont(font);

        switch (data_.type) {
            case NeuronType::COMMON:
                symbol = "C";
                break;
            case NeuronType::INPUT:
                symbol = "I";
                break;
            case NeuronType::OUTPUT:
                symbol = "O";
                break;
        }

        painter->drawText(boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, symbol);
    }
};
