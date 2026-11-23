#pragma once
#include <BrainPoint.h>
#include <../../../Brain/Model/include/NeuronType.h>
#include <string>

struct Data {
    virtual ~Data() {}
};

struct DendriteData : Data {
    QPointF pos;
    Uuid neuron;
    std::unordered_set<Uuid, UuidHash> synapses;
};

struct NeuronData : Data {
    QPointF pos;
    Uuid dendrite;
    Uuid axon;

    NeuronType type;

    double signal{};

    // for input neuron
    double intens{};
};

struct AxonNodeData {
    QPointF pos;
    int prev_node{-2}; // -1 - корневой нод (от нейрона); индекс предыдущего нейрона
    Uuid synaps;
};
struct AxonData : Data {
    Uuid neuron;
    std::vector<AxonNodeData> nodes;
};

struct SynapsData : Data {
    QPointF pos;
    Uuid axon;
    Uuid dendrite;

    double mediator{};
    double Ca{};
    double w{};
};

struct ProteinData : Data {
    QPointF pos;
    double dist;
};