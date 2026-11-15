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
        if (!(lod * UIConfig::synaps_info_font_size >= 3.0))   // отсекает мелкий размер и NaN
            return;

        const QString symbol =
            "C:" + QString::number(data_.Ca, 'f', 2) +
            "\nM:" + QString::number(data_.mediator, 'f', 2) +
            "\nW:" + QString::number(data_.w, 'f', 2);

        const QRectF r = boundingRect();
        const QPointF center = r.center();

        painter->save();

        // на время рисования текста убираем масштаб сцены: шрифт получает нормальный экранный размер
        painter->translate(center);
        painter->scale(1.0 / lod, 1.0 / lod);
        painter->translate(-center);

        QFont font = painter->font();
        font.setPointSizeF(UIConfig::synaps_info_font_size * lod);
        font.setHintingPreference(QFont::PreferNoHinting);
        painter->setFont(font);
        painter->setPen(QPen{QColorConstants::Black});

        // прямоугольник для переноса слов увеличиваем на lod, как и шрифт
        QRectF textRect{0, 0, r.width() * lod, r.height() * lod};
        textRect.moveCenter(center);

        painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, symbol);

        painter->restore();
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
