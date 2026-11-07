#pragma once

#include <SenseGetter.h>
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


public:
    explicit WorldController(WorldStorage &storage, WorldDataCollector &collector, WorldExecutor *executor)
        : storage_(storage), getter_(storage), collector_(collector), executor_(executor) {

    }

    // adders
    std::string addAgentBody(AgentBodyConfig conf) const;
    std::string addObstacle(QPointF, QRectF rect, QColor color) const;

    // changers
    void remove(const std::string &id);
    void setVel(const std::string &id, const b2Vec2 vec) const {
        const auto obj = storage_.getObject(id);
        const auto body = obj->getBodyId();

        b2Body_ApplyForceToCenter(body, vec, true);
    }
};
