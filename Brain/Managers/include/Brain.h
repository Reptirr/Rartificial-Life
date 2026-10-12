#pragma once
#include <Axon.h>
#include <Dendrite.h>
#include <Executor.h>
#include <Neuron.h>
#include <Storage.h>
#include <Synaps.h>

class Brain {
    Storage storage_;
    Executor executor_;


public:
    Brain() : executor_(storage_) {

    }

    Storage getCopyOfStorage() {
        return storage_;
    }

    std::unordered_map<std::string, std::string> createNeuron() {
        auto *axon = new Axon(&executor_);
        auto axon_id = storage_.put(axon);

        auto *neuron = new Neuron(&executor_, axon_id);
        auto neuron_id = storage_.put(neuron);

        auto *dendrite = new Dendrite(&executor_, neuron_id);
        auto dendrite_id = storage_.put(dendrite);


        return {
            {"neuron", neuron_id},
            {"axon", axon_id},
            {"dendrite", dendrite_id}
        };
    }

    std::string createSynaps(const std::string& axon_id, const std::string& dendrite_id) {
        auto *synaps = new Synaps(&executor_, dendrite_id);
        auto synaps_id = storage_.put(synaps);

        storage_.get<Axon>(axon_id)->addSynaps(synaps_id);

        return synaps_id;
    }

    void tact() {
        executor_.tact();
    }
};
