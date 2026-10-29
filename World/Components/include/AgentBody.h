#pragma once
#include <qcolor.h>
#include <qpoint.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {

    std::vector<QPointF> eyes = {{}};
    double angle{};
    double fov{std::numbers::pi}; // 180 градусов
    int rays_{};

public:
    explicit AgentBody(const int rays) : rays_(rays) {

    }

    std::vector<QColor> getVision(const int eye_i) {
        return sense_getter_->getVision(
            eyes[eye_i],
            angle,
            fov,
            rays_,
            100,
            QColorConstants::Cyan,
            this_id_
        );
    }

    void setVel(const double v) const {
        move_controller_->setVel(this_id_, v);
    }
};
