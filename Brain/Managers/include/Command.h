#pragma once
#include <string>
#include <unordered_map>

enum CommandType {
    NONE,

    NEURON_SPIKE,
    AXON_TRANSMITS_SYNAPS,
    SYNAPS_FORWARD_SIGNAL,
    DENDRIT_GET_SIGNAL,
    NEURON_GET_SIGNAL
};

struct Command {
    CommandType type{};
    std::unordered_map<std::string, std::string> data{};
};