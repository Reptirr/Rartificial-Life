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


    const std::unordered_map<std::string, Agent *> &agents() {
        return agents_;
    }
};
