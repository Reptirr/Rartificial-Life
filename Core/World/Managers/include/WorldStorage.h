#pragma once

#include <Food.h>
#include <string>
#include <unordered_map>
#include <Uuid.h>
#include <WorldObject.h>

class AgentBody;

class WorldStorage {
    /*
     * агенты
     * еда
     * враги
     * препятствия
     */

    b2WorldId world_id_;

    std::unordered_map<Uuid, WorldObject *, UuidHash> objects_;

    std::unordered_map<Uuid, AgentBody *, UuidHash> agent_bodies_;
    std::unordered_map<Uuid, Food *, UuidHash> food_;

public:
    explicit WorldStorage(const b2WorldId &world_id)
        : world_id_(world_id) {
    }

    Uuid putObject(WorldObject *obj) {
        auto id = Uuid::generate();
        objects_.insert({id, obj});
        return id;
    }
    WorldObject *getObject(const Uuid &id) const {
        return objects_.at(id);
    }
    bool removeObject(const Uuid &id) {
        return objects_.erase(id);
    }

    void putAgentBody(AgentBody *agent_body, Uuid id) {
        agent_bodies_.insert({id, agent_body});
    }
    AgentBody *getAgentBody(const Uuid &id) const {
        return agent_bodies_.at(id);
    }
    bool removeAgentBody(const Uuid &id) {
        return agent_bodies_.erase(id);
    }

    void putFood(Food *food, const Uuid& id) {
        food_.insert({id, food});
    }
    Food *getFood(const Uuid &id) const {
        return food_.at(id);
    }
    bool removeFood(const Uuid &id) {
        return food_.erase(id);
    }

    WorldEntity *getEntity(const Uuid &id);

    const std::unordered_map<Uuid, WorldObject *, UuidHash> &getObjects() const {
        return objects_;
    }
    const std::unordered_map<Uuid, Food *, UuidHash> &getFood() const {
        return food_;
    }
    const b2WorldId &getWorldId() const {
        return world_id_;
    }
};
