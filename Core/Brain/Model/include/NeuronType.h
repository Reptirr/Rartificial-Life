#pragma once

enum class NeuronType {
    // for common
    COMMON,

    // for input neurons
    INPUT,

    // for output neurons
    OUTPUT
};

static inline std::string_view toString(NeuronType type) {
    return magic_enum::enum_name(type);
}