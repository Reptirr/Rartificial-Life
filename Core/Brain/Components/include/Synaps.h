#pragma once
#include <Command.h>
#include <Gen.h>
#include <NeuronType.h>
#include <utility>
#include <Uuid.h>

class Dendrite;

class Synaps : public Executable, public BrainComponent {
    Uuid axon_;
    Uuid dendrite_;

    double w{1};

    int tick_before{};

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

    void reduce(int tick_i) {
        my_assert(prev_neuron_type_ != NeuronType::INPUT);

        int tick_delta = tick_i - tick_before;
        tick_before = tick_i;

        // чем больше Ca, тем быстрее уходит (экспоненциальный распад)
        constexpr double ca_min_step = 0.02; // минимальный шаг, чтобы значение дошло до нуля

        if (Ca != 0.0) {
            Ca -= std::max(Ca * ca_decay * tick_delta, ca_min_step);
            if (Ca < 0.0) Ca = 0.0;
        }

        if (mediator != 0.0) mediator = std::max(0.0, mediator - med_reduce_val * tick_delta);

        data_collector_->onUpdate(this_id_);
    }

protected:
    void onSetThisId() override {
        data_collector_->onCreate(this_id_);
    }

public:
    explicit Synaps(Executor *executor, Uuid dendrite, Uuid axon, NeuronType prev_neuron_type)
        : Executable(executor), axon_(axon), dendrite_(dendrite) {
        prev_neuron_type_ = prev_neuron_type;
    }

    void handleProtein(SubMap &sub_map, const Genome &genome) override {
        const auto proteins = sub_map.subsAtPoint(point_);

        GenBlock genblock;
        if (proteins.size() == 0) {
            genblock = genome.genblocks.at(0);
        } else {
            genblock = genome.genblocks.at(proteins.at(0)->genblock_i);
        }

        auto dscr = [&](std::string s) {
            if (genblock.contains(s)) return std::stod(genblock.at(s));
            return std::stod(genome.genblocks.at(0).at(s));
        };

        w_reduce_lr = dscr(Gen::SYNAPS_W_REDUCE_LR);
        w_produce_lr = dscr(Gen::SYNAPS_W_PRODUCE_LR);
        w_min = dscr(Gen::SYNAPS_W_MIN);
        w_max = dscr(Gen::SYNAPS_W_MAX);
        w = dscr(Gen::SYNAPS_W_INIT);
        ca_decay = dscr(Gen::SYNAPS_CA_DECAY);
        med_reduce_val = dscr(Gen::SYNAPS_MED_REDUCE_VAL);

    }

    void handleSpike(int tick_i) {
        executor_->push(Command{
            .type = DENDRIT_GET_SIGNAL,
            .data = {
                {"dendrite", dendrite_},
                {"signal", std::to_string(w)}
            }
        });

        if (prev_neuron_type_ == NeuronType::INPUT) return;
        reduce(tick_i);

        mediator += 1.0; // from prev neuron, imitation

        if (Ca > 0.0) {
            w -= mediator * w_reduce_lr;
            w = std::clamp(w, w_min, w_max);
        }

        data_collector_->onUpdate(this_id_);
    }

    void handleCa(int tick_i) {
        if (prev_neuron_type_ == NeuronType::INPUT) return;
        reduce(tick_i);


        Ca += 1.0;
        if (mediator > 0.0) {
            w += Ca * w_produce_lr;
            w = std::clamp(w, 0.0, 100.0);
        }

        data_collector_->onUpdate(this_id_);
    }

    // getters
    Uuid getAxon() const {
        return axon_;
    }
    Uuid getDendrite() const {
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