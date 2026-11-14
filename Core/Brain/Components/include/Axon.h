#pragma once
#include <BrainComponent.h>
#include <BrainMap.h>
#include <Command.h>
#include <DataCollector.h>
#include <Executable.h>
#include <Executor.h>
#include <random>
#include <RandomGenerator.h>
#include <string>
#include <unordered_set>
#include <utility>

struct AxonNode {
    BrainPoint point{};
    bool active{}; // при соединению с синапсам равен false
    std::string synaps{}; // "" если у него синапса нет, айди синапса если есть
    int previous_node{-1}; // индекс в векторе

    std::string toString() const {
        return "AxonNode{point=(" + std::to_string(point.x) + ", " + std::to_string(point.y) + ")"
             + ", active=" + (active ? "true" : "false")
             + ", previous_node=" + std::to_string(previous_node) + "}";
    }
};

class Synaps;

class Axon : public Executable, public BrainComponent {
    SubMap *sub_map_{};

    std::unordered_set<std::string> synapses_{};
    std::string neuron_{};

    // growing
    std::vector<AxonNode> nodes_{};
    int current_active_node_i{0};
    bool is_growing{};

    double step{0.09};
    double angle{};
    double angle_deviation{};
    double default_step{};
    double step_deviation{};
    double search_dendrite_r{};
    int max_nodes = 200;
    std::unordered_set<std::string> move_proteins_;
    std::unordered_set<std::string> avoid_proteins_;



    static BrainPoint moveByAzimuth(const BrainPoint p, const double azimuth, const double length) {
        return { p.x + length * std::sin(azimuth),
                    p.y + length * std::cos(azimuth) };
    }

protected:
    void onSetThisId() override {
        nodes_.push_back({
            .point = point_,
            .active = true,
            .previous_node = -1 // корень
        });

        is_growing = true;
        executor_->pushPlan(2, {
            .type = AXON_GREW,
            .data = {
                {"axon", this_id_}
            }
        });
    }

public:
    explicit Axon(Executor *executor, std::string neuron) : Executable(executor), neuron_(std::move(neuron)) {

    }

    void handleProtein(SubMap &sub_map, const Genome &genome) override {
        sub_map_ = &sub_map;
        const auto proteins = sub_map.subsAtPoint(point_);

        GenBlock genblock;
        if (proteins.size() == 0) {
            genblock = genome.genblocks.at(0);
        } else {
            genblock = genome.genblocks.at(proteins.at(0)->genblock_i);
        }

        // move/avoid proteins
        move_proteins_ = genblock.axon_move_proteins;
        avoid_proteins_ = genblock.axon_avoid_proteins;

        auto dscr = [&](std::string s) {
            if (genblock.contains(s)) return std::stod(genblock.at(s));
            return std::stod(genome.genblocks.at(0).at(s));
        };

        angle_deviation = dscr(Gen::AXON_ANGLE_DEVIATION);
        default_step = dscr(Gen::AXON_DEFAULT_STEP);
        step_deviation = dscr(Gen::AXON_STEP_DEVIATION);
        search_dendrite_r = dscr(Gen::AXON_SEARCH_DENDRITE_R);
        max_nodes = dscr(Gen::AXON_MAX_NODES);
    }

    void addSynaps(const std::string& synaps, int node_i) {
        synapses_.insert(synaps);
        if (node_i >= 0)
            nodes_.at(node_i).synaps = synaps;
        is_growing = false;

        data_collector_->onUpdate(this_id_);
    }

    void putSpike() const {
        for (auto synaps : synapses_) {
            executor_->push(Command{
                .type = SYNAPS_GET_SPIKE,
                .data = {
                    {"synaps", synaps}
                }
            });
        }
    }

    void handleGrow() {
        if (is_growing) {
            if (nodes().size() >= max_nodes) {
                is_growing = false;
                return;
            }

            constexpr double influence = 0.1;

            bool has_influence = false;

            for (const auto* protein : sub_map_->subsAtPoint(point_)) {
                const auto id = protein->id;

                const bool move = move_proteins_.contains(id);
                const bool avoid = avoid_proteins_.contains(id);

                if (!move && !avoid)
                    continue;

                has_influence = true;

                const double dx = point_.x - protein->point.x;
                const double dy = point_.y - protein->point.y;

                double target_angle = std::atan2(-dx, -dy);

                // для избегания разворачиваем направление на 180
                if (avoid)
                    target_angle += std::numbers::pi;

                const double delta = std::remainder(target_angle - angle, 2*std::numbers::pi);

                angle += delta * influence;
            }

            if (!has_influence)
                angle = Rng::normal(angle, angle_deviation);

            angle = std::remainder(angle, 2*std::numbers::pi);

            // меняем направление на центр при выходе за границу
            if (!(point_.x >= -1.0 && point_.x <= 1.0 &&
                point_.y >= -1.0 && point_.y <= 1.0)) {

                double target_angle = std::atan2(-point_.x, -point_.y);
                double delta = std::remainder(
                    target_angle - angle,
                    2.0 * std::numbers::pi
                );

                angle += delta * 0.1;
            }

            // move
            step = Rng::normal(default_step, step_deviation);

            point_ = moveByAzimuth(point_, angle, step);

            nodes_.at(current_active_node_i).active = false;
            nodes_.push_back({
                .point = point_,
                .active = true,
                .previous_node = current_active_node_i
            });

            current_active_node_i++;

            // check for dendrites
            for (auto dendrite_id : brain_map_->searchDendrite(point(), search_dendrite_r)) {
                executor_->push({
                    .type = CREATE_SYNAPS,
                    .data = {
                        {"axon", this_id_},
                        {"dendrite", dendrite_id},
                        {"current_node_index", std::to_string(current_active_node_i)}
                    }
                });

                nodes_.at(current_active_node_i).active = false;
                data_collector_->onUpdate(this_id_);
                return;
            }

            // reproduce event
            executor_->pushPlan(2, {
                .type = AXON_GREW,
                .data = {
                    {"axon", this_id_}
                }
            });

            data_collector_->onUpdate(this_id_);
        }
    }


    // getters
    std::unordered_set<std::string> getSynapses() {
        return synapses_;
    }
    std::string getNeuron() {
        return neuron_;
    }
    std::vector<AxonNode> nodes() {
        return nodes_;
    }
};
