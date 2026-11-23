#pragma once
#include <Axon.h>
#include <BrainMap.h>
#include <Controller.h>
#include <DataCollector.h>
#include <Executor.h>
#include <Genome.h>
#include <Storage.h>
#include <typedefs.h>
#include <Uuid.h>

class Brain {
    Storage storage_;
    Executor executor_;
    Controller controller_;
    BrainMap brain_map_{&storage_};
    SubMap sub_map_{storage_};
    DataCollector data_collector_;

public:
    explicit Brain() : executor_(storage_, controller_), controller_(storage_, executor_, brain_map_, data_collector_, sub_map_) {

    }

    void init(const Genome &genome) {
        storage_.setGenome(genome);

        // выброс белков в карту сабов
        controller_.injectProteins(genome.proteins);

        // создание нейронов в протеинах
        controller_.createNeuronsInProteins(genome.proteins);
    }

    void setCreateCallback(const IdConsumer &cons) {
        data_collector_.setCreateConsumer(cons);
    }
    void setUpdateCallback(const IdConsumer &cons) {
        data_collector_.setUpdateConsumer(cons);
    }
    void resetCallbacks() {
        data_collector_.resetConsumers();
    }

    Storage &getStorage() {
        return storage_;
    }

    std::unordered_map<std::string, Uuid> createNeuron(BrainPoint p) {
        auto res = controller_.createCommonNeuron(p);
        return res;
    }
    std::unordered_map<std::string, Uuid> createInputNeuron(BrainPoint p) {
        return controller_.createInputNeuron(p);
    }
    std::unordered_map<std::string, Uuid> createOutputNeuron(BrainPoint p) {
        return controller_.createOutputNeuron(p);
    }



    unsigned int tact() {
        return executor_.tact();
    }
};
