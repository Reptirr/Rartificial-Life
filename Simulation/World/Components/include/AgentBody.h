#pragma once
#include <AgentBodyConfig.h>
#include <qcolor.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {
    AgentBodyConfig conf_;

    double angle{};

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

    void applyAcceleration(b2Vec2 b2_vec2) {
        move_controller_->setVel(this_id_, b2_vec2);
    }

    // getters
    double getAzimuth() const {
        return angle;
    }
};
