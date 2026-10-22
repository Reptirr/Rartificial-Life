#pragma once
#include <Storage.h>

class Controller {
    Storage &storage_;
    Executor &executor_;
    BrainMap &brain_map_;

    std::function<void(std::string, std::string, std::string)> new_neuron_callback_; // neuron, dendrite, axon
    std::function<void(std::string)> new_synaps_callback_; // synaps
    std::function<void(std::string)> update_axon_callback_;
    std::function<void(std::string)> update_dendrite_callback_;

public:
    Controller(Storage &storage, Executor &executor, BrainMap &brain_map);

    void setNewNeuronCallback(const std::function<void(std::string, std::string, std::string)> &new_neuron_callback);
    void setNewSynapsCallback(const std::function<void(std::string)> &new_synaps_callback);
    void setNewNodeAxonCallback(const std::function<void(std::string)> &new_node_axon_callback);

    std::unordered_map<std::string, std::string> createNeuron();
    std::string createSynaps(const std::string &dendrite_id, const std::string &axon_id, int axon_node_from);
    void onNewAxonNode(const std::string &axon_id) const;
};
