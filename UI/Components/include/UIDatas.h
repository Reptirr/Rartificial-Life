#pragma once
#include <BrainPoint.h>
#include <string>


struct DendriteData {
    QPointF pos;
    std::string neuron;
    std::unordered_set<std::string> synapses;
};

struct NeuronData {
    QPointF pos;
    std::string dendrite;
    std::string axon;
};

struct AxonNodeData {
    QPointF pos;
    int prev_node{-2}; // -1 - корневой нод (от нейрона); индекс предыдущего нейрона
    std::string synaps; // "" - синапса нет и наоборот
};
struct AxonData {
    std::string neuron;
    std::vector<AxonNodeData> nodes;
};

struct SynapsData {
    QPointF pos;
    std::string axon;
    std::string dendrite;
};