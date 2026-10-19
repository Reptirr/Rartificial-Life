#pragma once
#include <AxonItem.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <qpen.h>
#include <Synaps.h>
#include <UIComponent.h>

class DendriteItem : public QGraphicsPathItem, public UIComponent {
    Dendrite *dendrite_;

public:
    DendriteItem(Storage &storage, Dendrite *dendrite) : UIComponent(storage) {
        dendrite_ = dendrite;
        setZValue(UIConfig::dendrite_z);

        const QPen pen{QColorConstants::Red, UIConfig::dendrite_width};
        setPen(pen);
    }

    void UIUpdate() override {
        QPainterPath path{};

        auto neuron_p = toQPointF(storage_.get<Neuron>(dendrite_->getNeuron())->point());

        for (auto synaps_id : dendrite_->getSynapses()) {
            auto synaps_p = toQPointF(storage_.get<Synaps>(synaps_id)->point());

            path.moveTo(neuron_p);
            path.lineTo(synaps_p);
        }

        setPath(path);
    }
};
