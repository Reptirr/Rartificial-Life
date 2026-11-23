#pragma once
#include <UIDatas.h>
#include <unordered_map>
#include <Uuid.h>

class ProteinItem;
class UIComponent;
class NeuronItem;
class DendriteItem;
class AxonItem;
class SynapsItem;

class UIStorage {
    std::unordered_map<Uuid, DendriteItem *> dendrites_;
    std::unordered_map<Uuid, NeuronItem *> neurons_;
    std::unordered_map<Uuid, AxonItem *> axons_;
    std::unordered_map<Uuid, SynapsItem *> synapses_;

    std::unordered_map<Uuid, ProteinItem *> proteins_;

    template <typename Map>
    static UIComponent *find(const Map &map, const Uuid &id) {
        const auto it = map.find(id);
        return it != map.end() ? it->second : nullptr;  // *Item* -> UIComponent* неявно
    }
public:
    DendriteItem *getDendrite(const Uuid &dendrite_id) const {
        return dendrites_.at(dendrite_id);
    }
    NeuronItem *getNeuron(const Uuid &neuron_id) const {
        return neurons_.at(neuron_id);
    }
    AxonItem *getAxon(const Uuid &axon_id) const {
        return axons_.at(axon_id);
    }
    SynapsItem *getSynaps(const Uuid &synapse_id) const {
        return synapses_.at(synapse_id);
    }
    ProteinItem *getProtein(const Uuid &id) const {
        return proteins_.at(id);
    }

    DendriteItem *insertDendrite(const Uuid &dendrite_id, DendriteItem *dendrite) {
        dendrites_.insert_or_assign(dendrite_id, dendrite);
        return dendrite;
    }
    NeuronItem *insertNeuron(const Uuid &neuron_id, NeuronItem *neuron) {
        neurons_.insert_or_assign(neuron_id, neuron);
        return neuron;
    }
    AxonItem *insertAxon(const Uuid &axon_id, AxonItem *axon) {
        axons_.insert_or_assign(axon_id, axon);
        return axon;
    }
    SynapsItem *insertSynaps(const Uuid &synapse_id, SynapsItem *synapse) {
        synapses_.insert_or_assign(synapse_id, synapse);
        return synapse;
    }
    ProteinItem *insertProtein(const Uuid &id, ProteinItem *item) {
        proteins_.insert_or_assign(id, item);
        return item;
    }

    const std::unordered_map<Uuid, DendriteItem *> &getDendrites() const {
        return dendrites_;
    }
    const std::unordered_map<Uuid, NeuronItem *> &getNeurons() const {
        return neurons_;
    }
    const std::unordered_map<Uuid, AxonItem *> &getAxons() const {
        return axons_;
    }
    const std::unordered_map<Uuid, SynapsItem *> &getSynapses() const {
        return synapses_;
    }

    UIComponent *getComponent(const Uuid &id) const {
        if (auto *c = find(neurons_, id)) return c;
        if (auto *c = find(dendrites_, id)) return c;
        if (auto *c = find(axons_, id)) return c;
        if (auto *c = find(synapses_, id)) return c;
        if (auto *c = find(proteins_, id)) return c;
        return nullptr;
    }
};