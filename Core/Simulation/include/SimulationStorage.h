#pragma once
#include <Agent.h>
#include <string>
#include <unordered_map>
#include <Uuid.h>


class SimulationStorage {
    std::unordered_map<Uuid, Agent *, UuidHash> agents_;

public:
    void putAgent(Uuid id, Agent *agent) {
        agents_.insert({id, agent});
    }
    Agent *getAgent(const Uuid &id) const {
        return agents_.at(id);
    }
    Agent *removeAgent(const Uuid &id) {
        auto *agent = agents_.at(id);
        agents_.erase(id);
        return agent;
    }

    const std::unordered_map<Uuid, Agent *, UuidHash> &agents() {
        return agents_;
    }
};
