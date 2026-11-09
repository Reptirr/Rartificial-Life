#pragma once


#include <Model.h>
#include <SimulationStorage.h>
#include <string>

class World;

class SimulationController {
    World &world_;
    SimulationStorage &storage_;

public:
    explicit SimulationController(World &world, SimulationStorage &storage)
        : world_(world), storage_(storage) {
    }

    std::string createAgent(AgentDef &agent_def);

    void deleteAgent(const std::string &id) {
        auto *agent = storage_.removeAgent(id);
        delete agent;
    }

};
