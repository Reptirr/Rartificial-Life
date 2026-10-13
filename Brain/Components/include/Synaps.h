#pragma once
#include <utility>

#pragma once

class Dendrite;

class Synaps : public Executable {

    std::string axon_;
    std::string dendrite_;

public:
    explicit Synaps(Executor *executor, std::string dendrite, std::string axon)
    : Executable(executor), axon_(std::move(axon)), dendrite_(std::move(dendrite)) {

    }

    void handleSpike() {
        if (!dendrite_.empty()) {
            executor_->push(Command{
                .type = DENDRIT_GET_SIGNAL,
                .data = {
                    {"dendrite", dendrite_},
                    {"signal", "8.23"}
                }
            });
        }
    }

    // getters
    std::string getAxon() {
        return axon_;
    }
    std::string getDendrite() {
        return dendrite_;
    }
};
