#pragma once
#include <qcolor.h>
#include <qpoint.h>
#include <WorldController.h>
#include <WorldEntity.h>

class AgentBody : public WorldEntity {

    std::vector<QPointF> eyes = {{}};
    double angle{};
    double fov{std::numbers::pi}; // 180 градусов

public:
    std::vector<QColor> getVision(const int eye_i) {
        return sense_->getVision(
            eyes[eye_i],
            angle,
            fov,
            128,
            100,
            QColorConstants::Cyan,
            this_id_
        );
    }
};
