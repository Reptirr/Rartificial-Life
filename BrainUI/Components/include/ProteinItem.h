#pragma once
#include <qbrush.h>
#include <UIConfig.h>
#include <UIDatas.h>

class UIStorage;

class ProteinItem : public QGraphicsEllipseItem, public UIComponent {
    ProteinData data_;

public:
    ProteinItem(UIStorage &storage, const ProteinData &data) : QGraphicsEllipseItem(-data.dist, -data.dist, data.dist * 2, data.dist * 2), UIComponent(storage), data_(data) {
        setZValue(UIConfig::protein_z);

        ProteinItem::UIUpdate();
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<ProteinData *>(d)) {
            data_ = *data;
        }
    }
    void UIUpdate() override {
        QRadialGradient grad{QPointF{0, 0}, data_.dist};
        grad.setColorAt(0.0, QColor(128, 0, 128, 255));
        grad.setColorAt(1.0, QColor(128, 0, 128, 33));

        setPen({Qt::darkMagenta, 1});
        setBrush(grad);
    }

};
