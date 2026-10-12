#pragma once
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>
#include <unordered_set>


class Synaps;

class Axon : public Executable {
    std::unordered_set<std::string> synapses_;

public:
    explicit Axon(Executor *executor) : Executable(executor) {

    }

    void addSynaps(const std::string& synaps) {
        synapses_.insert(synaps);
    }

    void putSpike() const {
        for (auto synaps : synapses_) {
            executor_->push(Command{
                SYNAPS_FORWARD_SIGNAL,
                {
                    {"synaps", synaps}
                }
            });
        }
    }
};
