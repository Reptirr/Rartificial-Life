#pragma once
#include <Axon.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>
#include <UIDatas.h>

class UIMapper {
public:
    static std::unique_ptr<Data> convertNeuron(Neuron *neuron) {
        auto data = std::make_unique<NeuronData>();

        data->pos = toQPointF(neuron->point());
        data->dendrite = neuron->getDendrite();
        data->axon = neuron->getAxon();

        return data;
    }

    static std::unique_ptr<Data> convertDendrite(Dendrite *dendrite) {
        auto data = std::make_unique<DendriteData>();

        data->pos = toQPointF(dendrite->point());
        data->neuron = dendrite->getNeuron();
        data->synapses = dendrite->getSynapses();

        return data;
    }

    static std::unique_ptr<Data> convertAxon(Axon *axon) {
        auto data = std::make_unique<AxonData>();

        data->nodes.reserve(axon->nodes().size());
        data->neuron = axon->getNeuron();
        for (auto &node : axon->nodes()) {
            AxonNodeData node_data{};

            node_data.pos = toQPointF(node.point);
            node_data.synaps = node.synaps;
            node_data.prev_node = node.previous_node;

            data->nodes.push_back(node_data);
        }

        return data;
    }

    static std::unique_ptr<Data> convertSynaps(Synaps *synaps) {
        auto data = std::make_unique<SynapsData>();

        data->pos = toQPointF(synaps->point());
        data->dendrite = synaps->getDendrite();
        data->axon = synaps->getAxon();

        return data;
    }
};