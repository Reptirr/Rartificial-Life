#pragma once


#include <Model.h>
#include <SimulationStorage.h>
#include <string>

class World;


class SimulationController {
    World &world_;
    SimulationStorage &storage_;

    StringConsumer onAgentDeath;

public:
    explicit SimulationController(World &world, SimulationStorage &storage)
        : world_(world), storage_(storage) {
    }

    void setOnAgentDeathConsumer(const StringConsumer &cons) {
        onAgentDeath = cons;
    }

    std::string createAgent(AgentDef &agent_def);
    void deleteAgent(const std::string &id) {
        onAgentDeath(id); // deletes ui

        auto *agent = storage_.removeAgent(id);
        delete agent;
    }

};
