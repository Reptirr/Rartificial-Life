#pragma once

#include <BrainComponent.h>
#include <Command.h>
#include <DataCollector.h>
#include <Executable.h>
#include <Executor.h>
#include <unordered_set>
#include <Uuid.h>

class Neuron;

class Dendrite : public Executable, public BrainComponent {
    Uuid neuron_{};
    std::unordered_set<Uuid, UuidHash> synapses_{};

public:
    explicit Dendrite(Executor *executor, Uuid neuron)
        : Executable(executor), neuron_(neuron) {
    }

    void handleProtein(SubMap &sub_map, const Genome &genome) override {}

    void addSignal(std::string val) {
        executor_->push(Command{
            .type = NEURON_GET_SIGNAL,
            .data = {
                {"neuron", neuron_},
                {"signal", val}
            }
        });
    }

    void addSynaps(Uuid synaps) {
        synapses_.insert(synaps);

        data_collector_->onUpdate(this_id_);
    }

    void transmitCu() const {
        for (auto synaps : synapses_) {
            executor_->push({
                .type = HANDLE_CA_IN_SYNAPS,
                .data = {
                    {"synaps", synaps}
                }
            });
        }
    }

    // getters
    Uuid getNeuron() const {
        return neuron_;
    }
    std::unordered_set<Uuid, UuidHash> getSynapses() const {
        return synapses_;
    }
};