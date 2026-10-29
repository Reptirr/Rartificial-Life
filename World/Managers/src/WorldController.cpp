#include <AgentBody.h>
#include <WorldController.h>
#include <box2d/types.h>
#include <box2d/box2d.h>

std::string WorldController::addAgentBody(QPointF pos) const {
    auto bd = b2DefaultBodyDef();
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    auto c = b2Circle{{}, 5.0};
    b2CreateCircleShape(body, &sd, &c);

    auto world_obj = new WorldObject(QColorConstants::White, body);
    auto id = storage_.putObject(world_obj);

    auto agent_body = new AgentBody();
    storage_.putAgentBody(agent_body, id);

    world_obj->init(id);
    agent_body->init(id, &getter_);

    return id;
}

std::string WorldController::addObstacle(QPointF pos) const {
    auto bd = b2DefaultBodyDef();
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    auto poly = b2MakeBox(0.5, 0.5);
    b2CreatePolygonShape(body, &sd, &poly);

    auto obj = new WorldObject(QColorConstants::Black, body);
    auto id = storage_.putObject(obj);

    b2Body_SetUserData(body, obj);

    return id;
}
