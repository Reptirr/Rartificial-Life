#pragma once
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>

class Axon;

class Neuron : public Executable {
    std::string axon_{};
    std::string dendrite_{};

    double sum{};
    double threshold{10};

public:
    explicit Neuron(Executor *executor)
        : Executable(executor) {

    }

    /// use it for neuron creating: first create neuron, after create axon&dendrite, after use that method to finish init
    void init(std::string axon, std::string dendrite) {
        axon_ = std::move(axon);
        dendrite_ = std::move(dendrite);
    }

    void addSignal(const std::string& signal_s) {
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

    // getters
    std::string getAxon() {
        return axon_;
    }
    std::string getDendrite() {
        return dendrite_;
    }

};
