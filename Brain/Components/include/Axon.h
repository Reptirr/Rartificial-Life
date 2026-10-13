#pragma once
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>
#include <unordered_set>
#include <utility>


class Synaps;

class Axon : public Executable {
    std::unordered_set<std::string> synapses_{};
    std::string neuron_{};

public:
    explicit Axon(Executor *executor, std::string neuron) : Executable(executor), neuron_(std::move(neuron)) {

    }

    void addSynaps(const std::string& synaps) {
        synapses_.insert(synaps);
    }

    void putSpike() const {
        for (auto synaps : synapses_) {
            executor_->push(Command{
                .type = SYNAPS_FORWARD_SIGNAL,
                .data = {
                    {"synaps", synaps}
                }
            });
        }
    }

    // getters
    std::unordered_set<std::string> getSynapses() {
        return synapses_;
    }
    std::string getNeuron() {
        return neuron_;
    }
};
