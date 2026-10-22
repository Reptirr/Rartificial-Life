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

    SynapsData &getData() {
        return data_;
    }
};
