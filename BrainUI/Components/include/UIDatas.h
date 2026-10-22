#pragma once
#include <BrainPoint.h>
#include <string>

struct Data {
    virtual ~Data() {}
};

struct DendriteData : Data {
    QPointF pos;
    std::string neuron;
    std::unordered_set<std::string> synapses;
};

struct NeuronData : Data {
    QPointF pos;
    std::string dendrite;
    std::string axon;
};

struct AxonNodeData {
    QPointF pos;
    int prev_node{-2}; // -1 - корневой нод (от нейрона); индекс предыдущего нейрона
    std::string synaps; // "" - синапса нет и наоборот
};
struct AxonData : Data {
    std::string neuron;
    std::vector<AxonNodeData> nodes;
};

struct SynapsData : Data {
    QPointF pos;
    std::string axon;
    std::string dendrite;
};