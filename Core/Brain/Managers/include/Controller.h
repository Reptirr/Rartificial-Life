#pragma once

#include <Storage.h>
#include <Uuid.h>

class Controller {
    Storage &storage_;
    Executor &executor_;
    BrainMap &brain_map_;
    SubMap &sub_map_;
    DataCollector &data_collector_;

public:
    Controller(Storage &storage, Executor &executor, BrainMap &brain_map, DataCollector &data_collector, SubMap &sub_map);

    std::unordered_map<std::string, Uuid> createCommonNeuron(BrainPoint point);
    std::unordered_map<std::string, Uuid> createInputNeuron(BrainPoint point);
    std::unordered_map<std::string, Uuid> createOutputNeuron(BrainPoint point);
    Uuid createSynaps(const Uuid &dendrite_id, const Uuid &axon_id, int axon_node_from);

    void injectProteins(const std::vector<Protein> &proteins) const {
        for (auto protein : proteins) {
            auto id = storage_.putProtein(new Protein(protein));
            data_collector_.onCreate(id);
        }
    }
    void createNeuronsInProteins(const std::vector<Protein> &proteins) {
        for (auto protein : proteins) {
            for (int i = 0; i < protein.neurons_num; i++) {
                // uniform disk sampling
                double u = Rng::randReal(0, 1);
                double v = Rng::randReal(0, 1);

                auto r = protein.dist * std::sqrt(u);
                double angle = 2 * std::numbers::pi * v;

                auto p = protein.point + BrainPoint{
                    .x = r * std::cos(angle),
                    .y = r * std::sin(angle)
                };

                createCommonNeuron(p);
            }
        }
    }
};
