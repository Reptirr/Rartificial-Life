#pragma once
#include <Command.h>
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

protected:
    void onSetThisId() override {
        data_collector_->onCreate(this_id_);
    }

public:
    explicit Synaps(Executor *executor, std::string dendrite, std::string axon, NeuronType prev_neuron_type)
        : Executable(executor), axon_(std::move(axon)), dendrite_(std::move(dendrite)) {
        prev_neuron_type_ = prev_neuron_type;
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

        mediator += 1.0; // from prev neuron imitation

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
        constexpr double ca_decay = 0.1;     // доля, уходящая за такт
        constexpr double ca_min_step = 0.02; // минимальный шаг, чтобы значение дошло до нуля

        Ca -= std::max(Ca * ca_decay, ca_min_step);
        if (Ca < 0.0) Ca = 0.0;

        mediator = std::max(0.0, mediator - 0.2);

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
