#include <AgentBody.h>
#include <QRect>
#include <WorldController.h>
#include <box2d/types.h>
#include <box2d/box2d.h>

std::string WorldController::addAgentBody(AgentBodyConfig conf) const {
    auto bd = b2DefaultBodyDef();
    bd.type = b2_dynamicBody;
    bd.linearDamping = 0.001;
    bd.angularDamping = 0.4;
    bd.position = conf.pos;
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    sd.density = 1;
    sd.enableContactEvents = true;
    auto c = b2Circle{{}, 0.3};
    b2CreateCircleShape(body, &sd, &c);

    auto *world_obj = new WorldObject(QColorConstants::Green, body, collector_);
    b2Body_SetUserData(body, world_obj);
    auto id = storage_.putObject(world_obj);

    auto *agent_body = new AgentBody(conf);
    storage_.putAgentBody(agent_body, id);


    world_obj->init(id, EntityKind::AGENT_BODY);
    agent_body->init(id, &getter_, executor_, &collector_);

    return id;
}

std::string WorldController::addObstacle(QPointF pos, QRectF rect, QColor color) const {
    auto bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};
    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    sd.isSensor = false;
    auto poly = b2MakeBox(rect.width()/2, rect.height()/2);
    b2CreatePolygonShape(body, &sd, &poly);

    auto obj = new WorldObject(color, body, collector_);
    auto id = storage_.putObject(obj);

    obj->init(id, EntityKind::NONE);

    b2Body_SetUserData(body, obj);

    return id;
}

std::string WorldController::addFood(QPointF pos) const {
    auto bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    bd.position = {static_cast<float>(pos.x()), static_cast<float>(pos.y())};

    auto body = b2CreateBody(storage_.getWorldId(), &bd);

    auto sd = b2DefaultShapeDef();
    sd.isSensor = false;
    sd.density = 1;
    sd.enableContactEvents = true;
    auto c = b2Circle{{}, 0.06};
    b2CreateCircleShape(body, &sd, &c);

    auto *obj = new WorldObject(QColorConstants::DarkYellow, body, collector_);
    auto id = storage_.putObject(obj);
    b2Body_SetUserData(body, obj);

    auto *food = new Food();
    storage_.putFood(food, id);

    obj->init(id, EntityKind::FOOD);
    food->init(id, &getter_, executor_, &collector_);

    return id;
}

void WorldController::remove(const std::string &id) {
    // delete from ui
    collector_.onRemove(id);

    auto *obj = storage_.getObject(id);

    switch (obj->getEntityKind()) {
        case EntityKind::NONE:
            break;
        case EntityKind::AGENT_BODY: {

            auto *agentbody = storage_.getAgentBody(id);
            storage_.removeAgentBody(id);
            delete agentbody;
            break;
        }
        case EntityKind::FOOD: {
            auto *food = storage_.getFood(id);
            storage_.removeFood(id);
            delete food;
            break;
        }
    }
    storage_.removeObject(id);

    // delete b2box body
    b2DestroyBody(obj->getBodyId());

    // delete obj
    delete obj;
}
