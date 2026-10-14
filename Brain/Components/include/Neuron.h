#pragma once
#include <BrainComponent.h>
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <string>

#include "../../../my_assert.h"

class Axon;

class Neuron : public Executable, public BrainComponent {
    std::string axon_{};
    std::string dendrite_{};

    double sum{};
    double threshold{10};

    void spike() {
        executor_->push(Command{
            .type = NEURON_SPIKE,
            .data = {
                {"axon", axon_}
            }
        });

        executor_->push({
            .type = HANDLE_CA_IN_DENDRITE,
            .data = {
                {"dendrite", dendrite_}
            }
        });
    }

public:
    explicit Neuron(Executor *executor)
        : Executable(executor) {

    }

    /// use it for neuron creating: first create neuron, after create axon&dendrite, after use that method to finish init
    /// set id before it
    void init(std::string axon, std::string dendrite) {
        axon_ = std::move(axon);
        dendrite_ = std::move(dendrite);

        my_assert(!this_id_.empty());

        static std::random_device rd{};
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution dist{0, 25};

        executor_->pushPlan(dist(gen), {
            .type = RANDOM_NEURON_SPIKE,
            .data = {
                {"neuron", this_id_}
            }
        });
    }

    void addSignal(const std::string& signal_s) {
        double signal = std::stod(signal_s);
        sum += signal;

        if (sum >= threshold) {
            sum = 0;
            spike();
        }
    }

    void onRandomSpike() {
        spike();

        executor_->pushPlan(10, {
            .type = RANDOM_NEURON_SPIKE,
            .data = {
                {"neuron", this_id_}
            }
        });
    }

    // getters
    std::string getAxon() {
        return axon_;
    }
    std::string getDendrite() {
        return dendrite_;
    }

};
