#pragma once
#include <QGraphicsEllipseItem>
#include <qpen.h>
#include <UIComponent.h>
#include <UIConfig.h>
#include <QStyleOption>

class SynapsItem : public QGraphicsEllipseItem, public UIComponent {
    SynapsData data_;
public:
    explicit SynapsItem(UIStorage &storage, SynapsData data) : QGraphicsEllipseItem(-UIConfig::synaps_r, -UIConfig::synaps_r,
                                                                 UIConfig::synaps_r * 2, UIConfig::synaps_r * 2),
                                            UIComponent(storage) {
        data_ = data;
        setPos(data_.pos);

        setZValue(UIConfig::synaps_z);

        setPen({QColorConstants::Gray, penWidthFromWidth(UIConfig::synaps_r*2)});
        setBrush({QColorConstants::Gray});

        update();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QGraphicsEllipseItem::paint(painter, option, widget);

        const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
        if (lod * UIConfig::synaps_info_font_size < 3.0)   // отсекает мелкий размер и NaN
            return;

        // input or output text
        QPen pen{QColorConstants::Black};
        painter->setPen(pen);

        QString symbol{};

        auto font = painter->font();
        font.setPointSizeF(UIConfig::synaps_info_font_size);
        painter->setFont(font);

        symbol =
            "C:" + QString::number(data_.Ca, 'f', 2) +
            "\nM:" + QString::number(data_.mediator, 'f', 2) +
            "\nW:" + QString::number(data_.w, 'f', 2);

        painter->drawText(boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, symbol);
    }

    SynapsData &getData() {
        return data_;
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<SynapsData *>(d)) {
            data_ = *data;
        }
    }

    void UIUpdate() override {
        update();
    }
};
