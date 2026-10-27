#pragma once
#include <Axon.h>
#include <BrainMap.h>
#include <Controller.h>
#include <DataCollector.h>
#include <Executor.h>
#include <Genome.h>
#include <Storage.h>
#include <typedefs.h>

class Brain {
    Storage storage_;
    Executor executor_;
    Controller controller_;
    BrainMap brain_map_{&storage_};
    SubMap sub_map_{storage_};
    DataCollector data_collector_;

    void createRandomNeurons(int number) {
        for (int i = 0; i < number; i++)
            createNeuron();
    }

public:
    explicit Brain() : executor_(storage_, controller_), controller_(storage_, executor_, brain_map_, data_collector_, sub_map_) {

    }

    void init(const Genome &genome, const int numberOfInitNeurons) {
        storage_.setGenome(genome);

        // выброс белков в карту сабов
        controller_.injectProteins(genome.proteins);

        createRandomNeurons(numberOfInitNeurons);
    }

    void setCreateCallback(const StringConsumer &cons) {
        data_collector_.setCreateConsumer(cons);
    }
    void setUpdateCallback(const StringConsumer &cons) {
        data_collector_.setUpdateConsumer(cons);
    }
    Storage &getStorage() {
        return storage_;
    }

    std::unordered_map<std::string, std::string> createNeuron() {
        auto res = controller_.createCommonNeuron();
        return res;
    }
    std::unordered_map<std::string, std::string> createInputNeuron(BrainPoint p) {
        return controller_.createInputNeuron(p);
    }
    std::unordered_map<std::string, std::string> createOutputNeuron(BrainPoint p) {
        return controller_.createOutputNeuron(p);
    }



    unsigned int tact() {
        return executor_.tact();
    }
};
