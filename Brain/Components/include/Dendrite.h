#pragma once

#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>

class Neuron;

class Dendrite : Executable {
    std::string neuron_{};

public:
    explicit Dendrite(Executor *executor, std::string neuron)
        : Executable(executor), neuron_(std::move(neuron)) {
    }

    void getSignal(std::string val) {
        executor_->push(Command{
            .type = NEURON_GET_SIGNAL,
            .data = {
                {"neuron", neuron_},
                {"signal", val}
            }
        });
    }
};
