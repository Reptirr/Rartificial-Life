#pragma once
#include <Storage.h>

class Controller {
    Storage &storage_;
    Executor &executor_;

public:
    Controller(Storage &storage, Executor &executor);

    std::unordered_map<std::string, std::string> createNeuron();
    std::string createSynaps(const std::string &dendrite_id, const std::string &axon_id);
};
