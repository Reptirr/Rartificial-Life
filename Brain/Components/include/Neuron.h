#pragma once
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>

class Axon;

class Neuron : public Executable {
    std::string axon_;

    double sum{};
    double threshold{10};

public:
    Neuron(Executor *executor, std::string axon)
        : Executable(executor),
          axon_(std::move(axon)) {
    }

    void getSignal(const std::string& signal_s) {
        double signal = std::stod(signal_s);
        sum += signal;

        if (sum >= threshold) {
            sum = 0;
            executor_->push(Command{
                .type = NEURON_SPIKE,
                .data = {
                    {"axon", axon_}
                }
            });
        }
    }

};
