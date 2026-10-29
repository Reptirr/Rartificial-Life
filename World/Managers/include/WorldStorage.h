#pragma once

#include <string>
#include <unordered_map>
#include <UUIDGenerator.h>
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

    std::unordered_map<std::string, WorldObject *> objects_;
    std::unordered_map<std::string, AgentBody *> agent_bodies_;

public:
    explicit WorldStorage(const b2WorldId &world_id)
        : world_id_(world_id) {
    }

    std::string putObject(WorldObject *obj) {
        auto id = generateUuid();
        objects_.insert({id, obj});
        return id;
    }
    WorldObject *getObject(const std::string &id) const {
        return objects_.at(id);
    }

    void putAgentBody(AgentBody *agent_body, std::string id) {
        agent_bodies_.insert({id, agent_body});
    }
    AgentBody *getAgentBody(const std::string &id) const {
        return agent_bodies_.at(id);
    }

    const std::unordered_map<std::string, WorldObject *> &getObjects() const {
        return objects_;
    }
    const b2WorldId &getWorldId() const {
        return world_id_;
    }
};
