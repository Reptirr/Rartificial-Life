#include <AgentBody.h>
#include <QRect>
#include <WorldController.h>
#include <box2d/types.h>
#include <box2d/box2d.h>

std::string WorldController::addAgentBody(QPointF pos, int rays, double fov) const {
    auto bd = b2DefaultBodyDef();
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    auto c = b2Circle{{}, 5.0};
    b2CreateCircleShape(body, &sd, &c);

    auto world_obj = new WorldObject(QColorConstants::Green, body, collector_);
    auto id = storage_.putObject(world_obj);

    auto agent_body = new AgentBody(rays, fov);
    storage_.putAgentBody(agent_body, id);

    world_obj->init(id, EntityKind::AGENT_BODY);
    agent_body->init(id, &getter_, &move_);

    return id;
}

std::string WorldController::addObstacle(QPointF pos, QRectF rect, QColor color) const {
    auto bd = b2DefaultBodyDef();
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    auto poly = b2MakeBox(rect.width()/2, rect.height()/2);
    b2CreatePolygonShape(body, &sd, &poly);

    auto obj = new WorldObject(color, body, collector_);
    auto id = storage_.putObject(obj);

    obj->init(id, EntityKind::NONE);

    b2Body_SetUserData(body, obj);

    return id;
}
