#pragma once
#include <QGraphicsEllipseItem>
#include <qpen.h>
#include <UIComponent.h>
#include <UIConfig.h>

class SynapsItem : public QGraphicsEllipseItem, public UIComponent {
    SynapsData data_;
public:
    explicit SynapsItem(UIStorage &storage, SynapsData data) : QGraphicsEllipseItem(-UIConfig::synaps_r, -UIConfig::synaps_r,
                                                                 UIConfig::synaps_r * 2, UIConfig::synaps_r * 2),
                                            UIComponent(storage) {
        data_ = data;
        setPos(data_.pos);

        setZValue(UIConfig::synaps_z);

        setPen({QColorConstants::Gray, 0.007});
        setBrush({QColorConstants::Gray});

        update();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QGraphicsEllipseItem::paint(painter, option, widget);


        // input or output text
        QPen pen{QColorConstants::Black};
        painter->setPen(pen);

        QString symbol{};

        auto font = painter->font();
        font.setPointSizeF(UIConfig::synaps_info_font_size);
        painter->setFont(font);

        symbol =
            "C:" + QString::number(data_.Ca, 'f', 2) +
            " M:" + QString::number(data_.mediator, 'f', 2) +
            " W:" + QString::number(data_.w, 'f', 3);

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
