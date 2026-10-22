#pragma once
#include <Storage.h>

class Controller {
    Storage &storage_;
    Executor &executor_;
    BrainMap &brain_map_;
    DataCollector &data_collector_;

public:
    Controller(Storage &storage, Executor &executor, BrainMap &brain_map, DataCollector &data_collector);

    std::unordered_map<std::string, std::string> createNeuron();
    std::string createSynaps(const std::string &dendrite_id, const std::string &axon_id, int axon_node_from);
};
