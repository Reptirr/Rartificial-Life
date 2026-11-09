#pragma once

#include <SenseGetter.h>
#include <SimulationController.h>
#include <WorldDataCollector.h>
#include <WorldExecutor.h>
#include <WorldStorage.h>
#include <box2d/box2d.h>

class AgentLifeController;
class QRectF;

class WorldController {
    WorldStorage &storage_;
    SenseGetter getter_;
    WorldDataCollector &collector_;
    WorldExecutor *executor_;
    double dt_;

    SimulationController *sim_controller_{};

public:
    explicit WorldController(WorldStorage &storage, WorldDataCollector &collector, WorldExecutor *executor, const double dt)
        : storage_(storage), getter_(storage), collector_(collector), executor_(executor), dt_(dt) {

    }

    void init(SimulationController *controller) {
        sim_controller_ = controller;
    }

    // adders
    std::string addAgentBody(AgentBodyConfig conf) const;
    std::string addObstacle(QPointF, QRectF rect, QColor color) const;
    std::string addFood(QPointF) const;

    // changers
    void remove(const std::string &id);
    void setVel(const std::string &id, const b2Vec2 vec) const {
        const auto obj = storage_.getObject(id);

        const auto body = obj->getBodyId();
        const b2Vec2 world = b2Body_GetWorldVector(body, vec);  // учитывает поворот тела

        b2Body_ApplyForceToCenter(body, world, true);
    }
    void rotate(const std::string &id, const double force) const {
        const auto obj = storage_.getObject(id);

        b2Body_ApplyAngularImpulse(obj->getBodyId(), force / dt_, true);
    }
};
