#pragma once
#include <Genome.h>
#include <vector>

enum class BrainAlign {
    Top,
    Right,
    Bottom,
    Left,

    Top_Left,
    Top_Right,

    Right_Top,
    Right_Bottom,

    Bottom_Left,
    Bottom_Right,

    Left_Top,
    Left_Bottom
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