#pragma once

#include <Storage.h>

class Controller {
    Storage &storage_;
    Executor &executor_;
    BrainMap &brain_map_;
    DataCollector &data_collector_;

public:
    Controller(Storage &storage, Executor &executor, BrainMap &brain_map, DataCollector &data_collector);

    std::unordered_map<std::string, std::string> createCommonNeuron();
    std::unordered_map<std::string, std::string> createInputNeuron(BrainPoint point);
    std::unordered_map<std::string, std::string> createOutputNeuron(BrainPoint point);
    std::string createSynaps(const std::string &dendrite_id, const std::string &axon_id, int axon_node_from);

    void injectProteins(const std::vector<Protein> &proteins) const {
        for (auto protein : proteins) {
            auto id = storage_.putProtein(new Protein(protein));
            data_collector_.onCreate(id);
        }
    }
};
