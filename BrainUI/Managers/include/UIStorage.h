#pragma once
#include <UIDatas.h>
#include <unordered_map>

class ProteinItem;
class UIComponent;
class NeuronItem;
class DendriteItem;
class AxonItem;
class SynapsItem;

class UIStorage {
    std::unordered_map<std::string, DendriteItem *> dendrites_;
    std::unordered_map<std::string, NeuronItem *> neurons_;
    std::unordered_map<std::string, AxonItem *> axons_;
    std::unordered_map<std::string, SynapsItem *> synapses_;

    std::unordered_map<std::string, ProteinItem *> proteins_;

    template <typename Map>
    static UIComponent *find(const Map &map, const std::string &id) {
        const auto it = map.find(id);
        return it != map.end() ? it->second : nullptr;  // *Item* -> UIComponent* неявно
    }
public:
    DendriteItem *getDendrite(const std::string &dendrite_id) const {
        return dendrites_.at(dendrite_id);
    }
    NeuronItem *getNeuron(const std::string &neuron_id) const {
        return neurons_.at(neuron_id);
    }
    AxonItem *getAxon(const std::string &axon_id) const {
        return axons_.at(axon_id);
    }
    SynapsItem *getSynaps(const std::string &synapse_id) const {
        return synapses_.at(synapse_id);
    }
    ProteinItem *getProtein(const std::string &id) const {
        return proteins_.at(id);
    }

    DendriteItem *insertDendrite(const std::string &dendrite_id, DendriteItem *dendrite) {
        dendrites_.insert_or_assign(dendrite_id, dendrite);
        return dendrite;
    }
    NeuronItem *insertNeuron(const std::string &neuron_id, NeuronItem *neuron) {
        neurons_.insert_or_assign(neuron_id, neuron);
        return neuron;
    }
    AxonItem *insertAxon(const std::string &axon_id, AxonItem *axon) {
        axons_.insert_or_assign(axon_id, axon);
        return axon;
    }
    SynapsItem *insertSynaps(const std::string &synapse_id, SynapsItem *synapse) {
        synapses_.insert_or_assign(synapse_id, synapse);
        return synapse;
    }
    ProteinItem *insertProtein(const std::string &id, ProteinItem *item) {
        proteins_.insert_or_assign(id, item);
        return item;
    }

    const std::unordered_map<std::string, DendriteItem *> &getDendrites() const {
        return dendrites_;
    }
    const std::unordered_map<std::string, NeuronItem *> &getNeurons() const {
        return neurons_;
    }
    const std::unordered_map<std::string, AxonItem *> &getAxons() const {
        return axons_;
    }
    const std::unordered_map<std::string, SynapsItem *> &getSynapses() const {
        return synapses_;
    }

    UIComponent *getComponent(const std::string &id) const {
        if (auto *c = find(neurons_, id)) return c;
        if (auto *c = find(dendrites_, id)) return c;
        if (auto *c = find(axons_, id)) return c;
        if (auto *c = find(synapses_, id)) return c;
        if (auto *c = find(proteins_, id)) return c;
        return nullptr;
    }
};