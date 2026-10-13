#pragma once
#include <Axon.h>
#include <Dendrite.h>
#include <Executor.h>
#include <Neuron.h>
#include <Storage.h>
#include <Synaps.h>

class Brain {
    Storage storage_{};
    Executor executor_;

    void createRandomNeurons(int number) {
        for (int i = 0; i < number; i++)
            createNeuron();
    }

public:
    explicit Brain(int numberOfInitNeurons) : executor_(storage_) {
        createRandomNeurons(numberOfInitNeurons);
    }

    Storage &getStorage() {
        return storage_;
    }

    std::unordered_map<std::string, std::string> createNeuron() {
        auto *neuron = new Neuron(&executor_);
        auto neuron_id = storage_.put(neuron);

        auto *axon = new Axon(&executor_, neuron_id);
        auto axon_id = storage_.put(axon);

        auto *dendrite = new Dendrite(&executor_, neuron_id);
        auto dendrite_id = storage_.put(dendrite);

        neuron->init(axon_id, dendrite_id);

        // random coordinates
        static std::random_device rd{};
        static std::mt19937 gen{rd()};
        static std::uniform_real_distribution dist{-1.0, 1.0};

        neuron->setPoint({ dist(gen), dist(gen) });
        axon->setPoint(neuron->point());
        dendrite->setPoint(neuron->point());

        return {
            {"neuron", neuron_id},
            {"axon", axon_id},
            {"dendrite", dendrite_id}
        };
    }

    std::string createSynaps(const std::string& axon_id, const std::string& dendrite_id) {
        auto *synaps = new Synaps(&executor_, dendrite_id, axon_id);
        auto synaps_id = storage_.put(synaps);

        auto *axon = storage_.get<Axon>(axon_id);
        auto *dendrite = storage_.get<Dendrite>(dendrite_id);
        auto *neuron1 = storage_.get<Neuron>(storage_.get<Axon>(axon_id)->getNeuron());
        auto *neuron2 = storage_.get<Neuron>(storage_.get<Dendrite>(dendrite_id)->getNeuron());

        axon->addSynaps(synaps_id);
        dendrite->addSynaps(synaps_id);

        return synaps_id;
    }

    unsigned int tact() {
        return executor_.tact();
    }
};
