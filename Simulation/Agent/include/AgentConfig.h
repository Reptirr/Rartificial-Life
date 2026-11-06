#pragma once
#include <Genome.h>
#include <vector>

enum class BrainAlign {
    Top,
    Right,
    Bottom,
    Left
};

struct NeuronGroup {
    int num{};
    BrainAlign align{};
};

struct AgentConfig {
    Genome genome;
    int num_of_common_neurons{};

    std::vector<NeuronGroup> input_groups;
    std::vector<NeuronGroup> output_groups;
};