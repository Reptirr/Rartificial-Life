#pragma once
#include <AxonItem.h>
#include <qpen.h>
#include <UIComponent.h>
#include <UIDatas.h>

class DendriteItem : public QGraphicsPathItem, public UIComponent {
    DendriteData data_;

public:
    DendriteItem(UIStorage &storage, const DendriteData &data) : UIComponent(storage) {
        data_ = data;
        setZValue(UIConfig::dendrite_z);

        const QPen pen{QColorConstants::Red, UIConfig::dendrite_width};
        setPen(pen);
    }

    void UIUpdate() override {
        QPainterPath path{};

        auto neuron_p = storage_.getNeuron(data_.neuron)->pos();

        for (auto synaps_id : data_.synapses) {
            auto synaps_p = storage_.getSynaps(synaps_id)->pos();

            path.moveTo(neuron_p);
            path.lineTo(synaps_p);
        }

        setPath(path);
    }

    void setData(DendriteData data) {
        data_ = data;
    }

    void setData(Data *d) override {
        if (const auto *data = dynamic_cast<DendriteData *>(d)) {
            data_ = *data;
        }
    }
};
