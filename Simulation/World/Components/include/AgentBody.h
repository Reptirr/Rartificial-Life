#pragma once
#include <qcolor.h>
#include <qpoint.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {

    std::vector<QPointF> eyes = {{}};
    double angle{};
    double fov_{std::numbers::pi}; // 180 градусов
    int rays_{};

public:
    explicit AgentBody(const int rays, double fov) : fov_(fov), rays_(rays) {

    }

    std::vector<QColor> getVision(const int eye_i) const {
        return sense_getter_->getVision(
            eyes[eye_i],
            angle,
            fov_,
            rays_,
            100,
            QColorConstants::Cyan,
            this_id_
        );
    }

    // getters
    double getAzimuth() const {
        return angle;
    }

    void applyAcceleration(b2Vec2 b2_vec2) {
        move_controller_->setVel(this_id_, b2_vec2);
    }
};
