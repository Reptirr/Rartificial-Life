#pragma once
#include <BrainComponent.h>
#include <Command.h>
#include <Executable.h>
#include <Executor.h>
#include <../../Model/include/NeuronType.h>
#include <random>
#include <string>

#include "../../../my_assert.h"

class Axon;

class Neuron : public Executable, public BrainComponent {
    std::string axon_{};
    std::string dendrite_{};
    NeuronType type_{};

    // настраиваемые свойства
    double threshold{10};
    int refractor_ticks = 5;
    int random_spike_ticks = 10;

    double sum{};
    bool can_spike{true};

    void spike() {
        if (!can_spike) return;

        can_spike = false;
        executor_->pushPlan(
            refractor_ticks,
            {
                .type = NEURON_REFRACTOR_RESET,
                .data = {
                    {"neuron", this_id_}
                }
            }
        );

        executor_->push(Command{
            .type = AXON_GET_SPIKE,
            .data = {
                {"axon", axon_}
            }
        });

        if (type_ != NeuronType::INPUT) {
            executor_->push({
                .type = HANDLE_CA_IN_DENDRITE,
                .data = {
                    {"dendrite", dendrite_}
                }
            });
        }
    }

public:
    explicit Neuron(Executor *executor, const NeuronType type)
        : Executable(executor), type_(type) {
    }

    /// use it for neuron creating: first create neuron, after create axon&dendrite, after use that method to finish init
    /// set id before it
    void init(std::string axon, std::string dendrite) {
        axon_ = std::move(axon);
        dendrite_ = std::move(dendrite);

        my_assert(!this_id_.empty());

        if (type_ == NeuronType::COMMON) {
            executor_->pushPlan(random_spike_ticks, {
                .type = RANDOM_NEURON_SPIKE,
                .data = {
                    {"neuron", this_id_}
                }
            });
        }

        data_collector_->onCreate(this_id_);
    }

    void handleProtein(const SubMap &sub_map, const Genome &genome) override {
        const auto proteins = sub_map.subsAtPoint(point_);

        GenBlock genblock;
        if (proteins.size() == 0) {
            genblock = genome.genblocks.at(0);
        } else {
            genblock = genome.genblocks.at(proteins.at(0)->genblock_i);
        }

        auto dscr = [&](std::string s) {
            return std::stod(genblock.at(s));
        };

        threshold = dscr(Gen::NEURON_THRESHOLD);
        refractor_ticks = dscr(Gen::NEURON_REFRACTOR_TICK);
        random_spike_ticks = dscr(Gen::NEURON_RANDOM_SPIKE_TICK);

    }

    void addSignal(const std::string& signal_s) {
        my_assert(type_ != NeuronType::INPUT);
        double signal = std::stod(signal_s);
        sum += signal;

        if (sum >= threshold) {
            sum = 0;
            spike();
        }

        data_collector_->onUpdate(this_id_);
    }

    void manualSpike() {
        my_assert(type_ == NeuronType::INPUT);
        spike();
    }
    void onRandomSpike() {
        spike();

        executor_->pushPlan(random_spike_ticks, {
            .type = RANDOM_NEURON_SPIKE,
            .data = {
                {"neuron", this_id_}
            }
        });
    }
    void refractorReset() {
        can_spike = true;
    }

    // getters
    std::string getAxon() {
        return axon_;
    }
    std::string getDendrite() {
        return dendrite_;
    }
    NeuronType getType() const {
        return type_;
    }
    double getSignal() const {
        return sum;
    }
};
