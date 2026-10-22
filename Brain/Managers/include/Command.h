#pragma once
#include <magic_enum.hpp>
#include <string>
#include <unordered_map>

enum CommandType : int {
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

    // GROWING
    AXON_GREW,
    CREATE_SYNAPS,

    // OTHER
    RANDOM_NEURON_SPIKE,

    // FOR UI
    AXON_NEW_NODE
};

inline std::string_view toString(CommandType type) {
    return magic_enum::enum_name(type);
}

struct Command {
    CommandType type{};
    std::unordered_map<std::string, std::string> data{};
};

struct PlanCommand {
    Command cmd{};
    int inTact{};
};