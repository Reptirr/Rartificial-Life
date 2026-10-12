#include <utility>

#pragma once

class Dendrite;

class Synaps : public Executable {

    std::string dendrite_;

public:
    explicit Synaps(Executor *executor, std::string dendrite) : Executable(executor), dendrite_(std::move(dendrite)) {

    }

    void getSpike() {
        if (!dendrite_.empty()) {
            executor_->push(Command{
                DENDRIT_GET_SIGNAL,
                {
                    {"dendrite", dendrite_},
                    {"signal", "1.23"}
                }
            });
        }
    }
};
