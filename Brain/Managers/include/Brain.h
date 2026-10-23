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
    DataCollector data_collector_{&storage_};

    void createRandomNeurons(int number) {
        for (int i = 0; i < number; i++)
            createNeuron();
    }

public:
    explicit Brain() : executor_(storage_, controller_), controller_(storage_, executor_, brain_map_, data_collector_) {

    }

    void init(int numberOfInitNeurons) {
        createRandomNeurons(numberOfInitNeurons);
    }

    void setNewNeuronCallback(const StringConsumer &cons) {
        data_collector_.setNewNeuronConsumer(cons);
    }
    void setNewSynapsCallback(const StringConsumer &cons) {
        data_collector_.setNewSynapsConsumer(cons);
    }
    void setUpdateDendriteCallback(const StringConsumer &cons) {
        data_collector_.setUpdateDendriteConsumer(cons);
    }
    void setUpdateAxonCallback(const StringConsumer &cons) {
        data_collector_.setUpdateAxonConsumer(cons);
    }

    Storage &getStorage() {
        return storage_;
    }

    std::unordered_map<std::string, std::string> createNeuron() {
        auto res = controller_.createNeuron();
        return res;
    }


    std::string createSynaps(const std::string& axon_id, const std::string& dendrite_id) {
        return controller_.createSynaps(dendrite_id, axon_id, -1);
    }

    unsigned int tact() {
        return executor_.tact();
    }
};
