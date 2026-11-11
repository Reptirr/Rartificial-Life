#pragma once

#include <AgentBodyConfig.h>
#include <AgentConfig.h>
#include <my_assert.h>
#include <qcolor.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {
    AgentBodyConfig conf_;

    std::vector<QColor> getVision() const {
        return sense_getter_->getVision(
            this_id_,
            conf_.fov,
            conf_.rays,
            100,
            QColorConstants::Cyan,
            this_id_
        );
    }

    // vars
    double energy{100};

public:
    explicit AgentBody(const AgentBodyConfig &conf)
        : conf_(conf) {

    }

    std::vector<NeuronGroup> getInputNeuronGroups() const {
        std::vector<NeuronGroup> groups;

        // vision
        groups.push_back({
            .num = conf_.rays*3,
            .align = BrainAlign::Left
        });

        return groups;
    }

    std::vector<NeuronGroup> getOutputNeuronGroups() const {
        std::vector<NeuronGroup> groups;

        // move
        groups.push_back({
            .num = conf_.move_dirs,
            .align = BrainAlign::Right_Top
        });

        // rotate
        groups.push_back({
            .num = 1,
            .align = BrainAlign::Right_Bottom
        });

        return groups;
    }

    // executes every tact
    std::vector<double> getInput() {
        std::vector<double> input;

        for (auto color : getVision()) {
            for (int i = 0; i < 3; i++) {
                double intens{};
                switch (i) {
                    case 0:
                        intens = color.redF();
                        break;
                    case 1:
                        intens = color.greenF();
                        break;
                    case 2:
                        intens = color.blueF();
                        break;
                }

                input.push_back(intens);
            }
        }

        // !костыль!
        energy -= 0.005;
        if (energy <= 0) {
            executor_->push({
                .type = WorldCommandType::AGENT_BODY_DEATH,
                .data = {
                    {"agentbody", this_id_}
                }
            });
        }

        return input;
    }
    void handleOutput(std::vector<bool> neurons) {
        my_assert(neurons.size() == conf_.move_dirs+1);

        int i = 0;

        // move
        b2Vec2 out_move{};
        for (; i < conf_.move_dirs; i++) {
            bool move = neurons[i];
            double move_angle = i / static_cast<double>(conf_.move_dirs) * std::numbers::pi*2;

            b2Vec2 dir = { .x = static_cast<float>(std::sin(move_angle)), .y = static_cast<float>(std::cos(move_angle)) };

            out_move += dir * move * 6;
        }
        if (out_move != b2Vec2{0, 0})
            executor_->push({
                .type = WorldCommandType::AGENT_BODY_MOVE,
                .data = {
                    {"agentbody", this_id_},
                    {"vec_x", std::to_string(out_move.x)},
                    {"vec_y", std::to_string(out_move.y)}
                }
            });

        // rotate
        if (neurons[i]) {
            executor_->push({
                .type = WorldCommandType::AGENT_BODY_ROTATE,
                .data = {
                    {"agentbody", this_id_},
                    {"force", std::to_string(3 * neurons[i])}
                }
            });
        }
    }

    void onEat() {
        energy += 10;
    }

    void onCollision(const WorldEntity *other) override {
        switch (other->kind()) {
            case EntityKind::FOOD: {
                executor_->push({
                    .type = WorldCommandType::AGENT_BODY_EAT_FOOD,
                    .data = {
                        {"agentbody", this_id_},
                        {"food", other->getId()}
                    }
                });

                return;
            }

            default:;
        }
    }

    EntityKind kind() const override {
        return EntityKind::AGENT_BODY;
    }

    // getters
    double getEnergy() const {
        return energy;
    }
};
