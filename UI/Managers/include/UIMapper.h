#pragma once
#include <Axon.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>
#include <UIDatas.h>

class UIMapper {
public:
    static NeuronData convertNeuron(Neuron *neuron) {
        NeuronData data{};

        data.pos = toQPointF(neuron->point());
        data.dendrite = neuron->getDendrite();
        data.axon = neuron->getAxon();

        return data;
    }

    static DendriteData convertDendrite(Dendrite *dendrite) {
        DendriteData data{};

        data.pos = toQPointF(dendrite->point());
        data.neuron = dendrite->getNeuron();
        data.synapses = dendrite->getSynapses();

        return data;
    }

    static AxonData convertAxon(Axon *axon) {
        AxonData data{};

        data.neuron = axon->getNeuron();
        for (auto &node : axon->nodes()) {
            AxonNodeData node_data{};

            node_data.pos = toQPointF(node.point);
            node_data.synaps = node.synaps;
            node_data.prev_node = node.previous_node;

            data.nodes.push_back(node_data);
        }

        return data;
    }

    static SynapsData convertSynaps(Synaps *synaps) {
        SynapsData data{};

        data.pos = toQPointF(synaps->point());
        data.dendrite = synaps->getDendrite();
        data.axon = synaps->getAxon();

        return data;
    }
};
