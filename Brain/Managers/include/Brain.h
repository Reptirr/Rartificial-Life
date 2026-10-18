#pragma once
#include <Axon.h>
#include <Controller.h>
#include <Executor.h>
#include <Neuron.h>
#include <Storage.h>

class Brain {
    Storage storage_{};
    Executor executor_;
    Controller controller_;
    BrainMap brain_map_{&storage_};


    void createRandomNeurons(int number) {
        for (int i = 0; i < number; i++)
            createNeuron();
    }

public:
    explicit Brain(int numberOfInitNeurons) : executor_(storage_, controller_), controller_(storage_, executor_, brain_map_) {
        createRandomNeurons(numberOfInitNeurons);
    }

    Storage &getStorage() {
        return storage_;
    }

    std::unordered_map<std::string, std::string> createNeuron() {
        return controller_.createNeuron();
    }


    std::string createSynaps(const std::string& axon_id, const std::string& dendrite_id) {
        return controller_.createSynaps(dendrite_id, axon_id, -1);
    }

    unsigned int tact() {
        return executor_.tact();
    }
};
