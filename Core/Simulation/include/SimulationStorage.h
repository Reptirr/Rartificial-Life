#pragma once
#include <Agent.h>
#include <string>
#include <unordered_map>

class SimulationStorage {
    std::unordered_map<std::string, Agent *> agents_;

public:
    void putAgent(std::string id, Agent *agent) {
        agents_.insert({id, agent});
    }
    Agent *getAgent(const std::string &id) const {
        return agents_.at(id);
    }
    Agent *removeAgent(const std::string &id) {
        auto *agent = agents_.at(id);
        agents_.erase(id);
        return agent;
    }

    const std::unordered_map<std::string, Agent *> &agents() {
        return agents_;
    }
};
