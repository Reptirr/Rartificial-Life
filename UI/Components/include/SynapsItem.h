#pragma once
#include <BrainPoint.h>
#include <QGraphicsEllipseItem>
#include <qgraphicsscene.h>
#include <qpen.h>
#include <Synaps.h>
#include <UIComponent.h>
#include <UIConfig.h>

class SynapsItem : public QGraphicsEllipseItem, public UIComponent {
    Synaps *synaps_;

public:
    explicit SynapsItem(Storage &storage, Synaps *synaps) : QGraphicsEllipseItem(-UIConfig::synaps_r, -UIConfig::synaps_r,
                                                                 UIConfig::synaps_r * 2, UIConfig::synaps_r * 2),
                                            UIComponent(storage),
                                            synaps_(synaps) {
        setPos(toQPointF(synaps->point()));

        setZValue(UIConfig::synaps_z);

        setPen({QColorConstants::Gray, 0.007});
        setBrush({QColorConstants::Gray});

        update();
    }
};
