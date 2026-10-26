#pragma once
#include <DendriteItem.h>
#include <ProteinItem.h>
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
    ProteinItem *createProteinItem(const ProteinData &data) const {
        auto *item = new ProteinItem(ui_storage_, data);
        return item;
    }

public:
    explicit UIController(UIStorage &ui_storage, QGraphicsScene *scene)
        : ui_storage_(ui_storage), scene_(scene) {
    }


    auto newNeuron(const std::string &neuron_id, const NeuronData &data) const {
        auto *item = createNeuronItem(data);
        ui_storage_.insertNeuron(neuron_id, item);
        addItems(scene_, {item});
        return item;
    }

    auto newDendrite(const std::string &dendrite_id, const DendriteData &data) const {
        auto *item = createDendriteItem(data);
        ui_storage_.insertDendrite(dendrite_id, item);
        addItems(scene_, {item});
        return item;
    }

    auto newAxon(const std::string &axon_id, const AxonData &data) const {
        auto *item = createAxonItem(data);
        ui_storage_.insertAxon(axon_id, item);
        addItems(scene_, {item});
        return item;
    }

    auto newSynaps(const std::string &synaps_id, const SynapsData &data) const {
        auto *item = createSynapsItem(data);
        ui_storage_.insertSynaps(synaps_id, item);
        scene_->addItem(item);
        return item;
    }

    auto newProtein(const std::string &protein_id, const ProteinData &data) const {
        auto *item = createProteinItem(data);
        ui_storage_.insertProtein(protein_id, item);
        scene_->addItem(item);
        return item;
    }

    void updateComponent(UIComponent *component, Data *data) {
        component->setData(data);
        component->UIUpdate();
    }
};
