#pragma once
#include <BrainComponent.h>
#include <BrainMap.h>
#include <Command.h>
#include <DataCollector.h>
#include <Executable.h>
#include <Executor.h>
#include <random>
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
        handleGrow();
    }

public:
    explicit Axon(Executor *executor, std::string neuron) : Executable(executor), neuron_(std::move(neuron)) {

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

            static std::random_device rd{};
            static std::mt19937 gen{rd()};

            std::normal_distribution angle_dist{0.0, angle_deviation};
            angle += angle_dist(gen);
            angle = std::remainder(angle, 2.0 * std::numbers::pi);

            std::normal_distribution step_dist{default_step, step_deviation};
            step = step_dist(gen);

            point_ = moveByAzimuth(point_, angle, step);

            nodes_.at(current_active_node_i).active = false;
            nodes_.push_back({
                .point = point_,
                .active = true,
                .previous_node = current_active_node_i
            });

            current_active_node_i++;

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
