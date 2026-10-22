#pragma once
#include <DendriteItem.h>
#include <qgraphicsscene.h>
#include <SceneUtils.h>
#include <SynapsItem.h>
#include <UIStorage.h>


class UIController {
    UIStorage &ui_storage_;
    QGraphicsScene *scene_;

    DendriteItem *createDendriteItem(const DendriteData &data) const {
        auto *item = new DendriteItem(ui_storage_, data);
        return item;
    }
    NeuronItem *createNeuronItem(const NeuronData &data) const {
        auto *item = new NeuronItem(ui_storage_, data);
        return item;
    }
    AxonItem *createAxonItem(const AxonData &data) const {
        auto *item = new AxonItem(ui_storage_, data);
        return item;
    }
    SynapsItem *createSynapsItem(const SynapsData &data) const {
        auto *item = new SynapsItem(ui_storage_, data);
        return item;
    }

public:
    explicit UIController(UIStorage &ui_storage, QGraphicsScene *scene)
        : ui_storage_(ui_storage), scene_(scene) {
    }


    void newNeuron(const std::string &neuron_id, const NeuronData &neuron_data, const DendriteData &dendrite_data, const AxonData &axon_data) const {
        auto *neuron_item = createNeuronItem(neuron_data);
        auto *dendrite_item = createDendriteItem(dendrite_data);
        auto *axon_item = createAxonItem(axon_data);

        ui_storage_.insertNeuron(neuron_id, neuron_item);
        ui_storage_.insertDendrite(neuron_data.dendrite, dendrite_item);
        ui_storage_.insertAxon(neuron_data.axon, axon_item);

        addItems(scene_, {
                     neuron_item,
                     dendrite_item,
                     axon_item
                 });
    }

    void newSynaps(const std::string &synaps_id, const SynapsData &data) {
        auto *synaps_item = createSynapsItem(data);

        ui_storage_.insertSynaps(synaps_id, synaps_item);

        scene_->addItem(synaps_item);
    }

    void newNodeAxon(const std::string &axon_id, const AxonData &data) {
        auto *axon_item = ui_storage_.getAxon(axon_id);

        axon_item->setData(data);
        axon_item->UIUpdate();
    }
};
