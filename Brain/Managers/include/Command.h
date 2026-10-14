#pragma once
#include <string>
#include <unordered_map>

enum CommandType {
    NONE,

    // BASE
    NEURON_SPIKE,
    SYNAPS_GET_SPIKE,
    DENDRIT_GET_SIGNAL,
    NEURON_GET_SIGNAL,

    // EDU
    HANDLE_CA_IN_DENDRITE,
    HANDLE_CA_IN_SYNAPS,
    REDUCE_VALS_IN_SYNAPS,

    // OTHER
    RANDOM_NEURON_SPIKE
};

struct Command {
    CommandType type{};
    std::unordered_map<std::string, std::string> data{};
};

struct PlanCommand {
    Command cmd{};
    int inTact{};


};