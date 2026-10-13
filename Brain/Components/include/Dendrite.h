#pragma once

#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>

class Neuron;

class Dendrite : public Executable {
    std::string neuron_{};
    std::unordered_set<std::string> synapses_{};

public:
    explicit Dendrite(Executor *executor, std::string neuron)
        : Executable(executor), neuron_(std::move(neuron)) {
    }

    void addSignal(std::string val) {
        executor_->push(Command{
            .type = NEURON_GET_SIGNAL,
            .data = {
                {"neuron", neuron_},
                {"signal", val}
            }
        });
    }

    void addSynaps(std::string synaps) {
        synapses_.insert(synaps);
    }

    // getters
    std::string getNeuron() {
        return neuron_;
    }
    std::unordered_set<std::string> getSynapses() {
        return synapses_;
    }
};
