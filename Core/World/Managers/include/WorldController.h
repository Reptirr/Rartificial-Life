#pragma once

#include <SenseGetter.h>
#include <SimulationBridge.h>
#include <SimulationController.h>
#include <WorldExecutor.h>
#include <WorldStorage.h>
#include <box2d/box2d.h>
#include <Uuid.h>

class AgentLifeController;
class QRectF;

class WorldController {
    WorldStorage &storage_;
    SenseGetter getter_;
    WorldDataCollector &collector_;
    WorldExecutor *executor_;
    double dt_;

    SimulationBridge *sim_bridge_{};

public:
    explicit WorldController(WorldStorage &storage, WorldDataCollector &collector, WorldExecutor *executor, const double dt)
        : storage_(storage), getter_(storage), collector_(collector), executor_(executor), dt_(dt) {

    }

    void init(SimulationBridge *bridge) {
        sim_bridge_ = bridge;
    }

    // adders
    void birthAgent(const Uuid &id) const;
    Uuid addAgentBody(AgentBodyConfig conf) const;
    Uuid addObstacle(QPointF, QRectF rect, QColor color) const;
    Uuid addFood(QPointF) const;

    // changers
    void remove(const Uuid &id);
    void setVel(const Uuid &id, b2Vec2 vec) const {
        const auto obj = storage_.getObject(id);
        const auto body = obj->getBodyId();

        const b2Vec2 world = b2Body_GetWorldVector(body, vec);
        b2Body_SetLinearVelocity(body, world);
    }
    void rotate(const Uuid &id, const double force) const {
        const auto obj = storage_.getObject(id);

        b2Body_ApplyAngularImpulse(obj->getBodyId(), force / dt_, true);
    }
};
