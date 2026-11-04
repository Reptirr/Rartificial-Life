#pragma once
#include <AgentBodyConfig.h>
#include <my_assert.h>
#include <qcolor.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {
    AgentBodyConfig conf_;

    double angle{};

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

public:
    explicit AgentBody(AgentBodyConfig conf) : conf_(conf) {

    }

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

        return input;
    }

    void handleOutput(std::vector<bool> neurons) {
        my_assert(neurons.size() == conf_.move_dirs);

        for (int i = 0; i < conf_.move_dirs; i++) {
            bool move = neurons[i];
            double move_angle = (i+1) / static_cast<double>(conf_.move_dirs) * std::numbers::pi*2 + angle;

            b2Vec2 dir = { .x = static_cast<float>(std::sin(move_angle)), .y = static_cast<float>(std::cos(move_angle)) };

            move_controller_->setVel(this_id_, dir * move);
        }
    }


    // getters
    double getAzimuth() const {
        return angle;
    }
};
