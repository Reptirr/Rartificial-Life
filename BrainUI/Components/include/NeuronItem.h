#pragma once

#include <QGraphicsEllipseItem>
#include <qpen.h>
#include <QStyleOption>
#include <SizeUtils.h>
#include <UIComponent.h>
#include <UIConfig.h>

class NeuronItem : public QGraphicsEllipseItem, public UIComponent {
    NeuronData data_;

public:
    NeuronItem(UIStorage &storage, const NeuronData &data) : QGraphicsEllipseItem(-UIConfig::neuron_r, -UIConfig::neuron_r,
                                                        UIConfig::neuron_r * 2, UIConfig::neuron_r * 2), UIComponent(storage) {
        data_ = data;
        setPos(data_.pos);

        setZValue(UIConfig::neuron_z);

        setPen(QPen{Qt::black, penWidthFromWidth(UIConfig::neuron_r*2)});
        setBrush(QBrush{QColorConstants::DarkCyan});
        update();
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<NeuronData *>(d)) {
            data_ = *data;
        }
    }

    void UIUpdate() override {
        update();
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QGraphicsEllipseItem::paint(painter, option, widget);

        const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
        const qreal screenFontSize = UIConfig::neuron_type_font_size * lod;   // размер шрифта в пикселях на экране

        constexpr qreal kMinReadableFontSize = 6.0;   // порог подбери глазами
        const bool showText = screenFontSize >= kMinReadableFontSize;

        QString symbol;
        switch (data_.type) {
            case NeuronType::COMMON: symbol = "C"; break;
            case NeuronType::INPUT:  symbol = "";  break;
            case NeuronType::OUTPUT: symbol = "O"; break;
        }
        symbol += "\nS: " + QString::number(data_.signal, 'f', 2);

        if (showText) {
            painter->save();

            const QRectF r = boundingRect();
            const QPointF center = r.center();

            // убираем масштаб на время рисования текста
            painter->translate(center);
            painter->scale(1.0 / lod, 1.0 / lod);
            painter->translate(-center);

            QFont font = painter->font();
            font.setPointSizeF(UIConfig::neuron_type_font_size * lod);   // нормальный экранный размер
            font.setHintingPreference(QFont::PreferNoHinting);
            painter->setFont(font);
            painter->setPen(QPen{QColorConstants::Black});

            // прямоугольник тоже нужно увеличить на lod, иначе перенос слов сработает не так
            QRectF textRect{0, 0, r.width() * lod, r.height() * lod};
            textRect.moveCenter(center);

            painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, symbol);
            painter->restore();
        }

        if (data_.type == NeuronType::INPUT) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(data_.color);
            const qreal s = UIConfig::neuron_type_font_size;
            const QPointF c = boundingRect().center();
            painter->drawRect(QRectF{c.x() - s / 2, c.y() - s / 2, s, s});
        }
    }
};
