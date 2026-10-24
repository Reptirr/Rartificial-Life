#pragma once
#include <Axon.h>
#include <Dendrite.h>
#include <Neuron.h>
#include <Synaps.h>
#include <UIDatas.h>

class UIMapper {
    int w{},h{};

    QPointF scr(BrainPoint p) {
        return toQPointF({p.x * w, p.y * h});
    }

public:
    UIMapper(int window_w, int window_h) {
        w = window_w;
        h = window_h;
    }

    std::unique_ptr<Data> convertNeuron(Neuron *neuron) {
        auto data = std::make_unique<NeuronData>();

        data->pos = scr(neuron->point());
        data->dendrite = neuron->getDendrite();
        data->axon = neuron->getAxon();
        data->type = neuron->getType();

        return data;
    }

    std::unique_ptr<Data> convertDendrite(Dendrite *dendrite) {
        auto data = std::make_unique<DendriteData>();

        data->pos = scr(dendrite->point());
        data->neuron = dendrite->getNeuron();
        data->synapses = dendrite->getSynapses();

        return data;
    }

    std::unique_ptr<Data> convertAxon(Axon *axon) {
        auto data = std::make_unique<AxonData>();

        data->nodes.reserve(axon->nodes().size());
        data->neuron = axon->getNeuron();
        for (auto &node : axon->nodes()) {
            AxonNodeData node_data{};

            node_data.pos = scr(node.point);
            node_data.synaps = node.synaps;
            node_data.prev_node = node.previous_node;

            data->nodes.push_back(node_data);
        }

        return data;
    }

    std::unique_ptr<Data> convertSynaps(Synaps *synaps) {
        auto data = std::make_unique<SynapsData>();

        data->pos = scr(synaps->point());
        data->dendrite = synaps->getDendrite();
        data->axon = synaps->getAxon();
        data->Ca = synaps->getCa();
        data->mediator = synaps->getMediator();
        data->w = synaps->getW();

        return data;
    }
};