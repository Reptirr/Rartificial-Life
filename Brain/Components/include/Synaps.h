#pragma once
#include <Command.h>
#include <Gen.h>
#include <utility>

#pragma once

class Dendrite;

class Synaps : public Executable, public BrainComponent {
    std::string axon_;
    std::string dendrite_;

    double w{1};

    bool is_reducing{};

    NeuronType prev_neuron_type_{};

    // edu
    double mediator{};
    double Ca{};

    // свойства
    double w_reduce_lr{};
    double w_produce_lr{};
    double w_min{};
    double w_max{};
    double ca_decay{};
    double med_reduce_val{};

protected:
    void onSetThisId() override {
        data_collector_->onCreate(this_id_);
    }

public:
    explicit Synaps(Executor *executor, std::string dendrite, std::string axon, NeuronType prev_neuron_type)
        : Executable(executor), axon_(std::move(axon)), dendrite_(std::move(dendrite)) {
        prev_neuron_type_ = prev_neuron_type;
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

        w_reduce_lr = dscr(Gen::SYNAPS_W_REDUCE_LR);
        w_produce_lr = dscr(Gen::SYNAPS_W_PRODUCE_LR);
        w_min = dscr(Gen::SYNAPS_W_MIN);
        w_max = dscr(Gen::SYNAPS_W_MAX);
        w = dscr(Gen::SYNAPS_W_INIT);
        ca_decay = dscr(Gen::SYNAPS_CA_DECAY);
        med_reduce_val = dscr(Gen::SYNAPS_MED_REDUCE_VAL);

    }

    void handleSpike() {
        executor_->push(Command{
            .type = DENDRIT_GET_SIGNAL,
            .data = {
                {"dendrite", dendrite_},
                {"signal", std::to_string(w)}
            }
        });

        if (prev_neuron_type_ == NeuronType::INPUT) return;

        mediator += 1.0; // from prev neuron, imitation

        if (Ca > 0.0) {
            w -= mediator * 0.05;
            w = std::clamp(w, 0.0, 100.0);
        }

        data_collector_->onUpdate(this_id_);

        if (!is_reducing) {
            executor_->push({
                .type = REDUCE_VALS_IN_SYNAPS,
                .data = {
                    {"synaps", this_id_}
                }
            });

            is_reducing = true;
        }
    }

    void handleCa() {
        if (prev_neuron_type_ == NeuronType::INPUT) return;

        Ca += 1.0;
        if (mediator > 0.0) {
            w += Ca * 0.05;
            w = std::clamp(w, 0.0, 100.0);
        }

        data_collector_->onUpdate(this_id_);

        if (!is_reducing) {
            executor_->push({
                .type = REDUCE_VALS_IN_SYNAPS,
                .data = {
                    {"synaps", this_id_}
                }
            });

            is_reducing = true;
        }
    }

    void reduce() {
        my_assert(prev_neuron_type_ != NeuronType::INPUT);

        // чем больше Ca, тем быстрее уходит (экспоненциальный распад)
        constexpr double ca_min_step = 0.02; // минимальный шаг, чтобы значение дошло до нуля

        Ca -= std::max(Ca * ca_decay, ca_min_step);
        if (Ca < 0.0) Ca = 0.0;

        mediator = std::max(0.0, mediator - med_reduce_val);

        data_collector_->onUpdate(this_id_);

        if (Ca != 0.0 || mediator != 0.0) {
            executor_->push({
                .type = REDUCE_VALS_IN_SYNAPS,
                .data = {
                    {"synaps", this_id_}
                }
            });
        } else {
            is_reducing = false;
        }
    }

    // getters
    std::string getAxon() {
        return axon_;
    }
    std::string getDendrite() {
        return dendrite_;
    }
    double getCa() {
        return Ca;
    }
    double getMediator() {
        return mediator;
    }
    double getW() {
        return w;
    }
};
