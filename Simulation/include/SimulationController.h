#pragma once


#include <Model.h>
#include <SimulationStorage.h>
#include <string>
#include <World.h>

class SimulationController {
    World &world_;
    SimulationStorage &storage_;
    AgentLifeController *life_controller_{};

public:
    explicit SimulationController(World &world, SimulationStorage &storage)
        : world_(world), storage_(storage) {
    }

    void init(AgentLifeController *life_controller) {
        life_controller_ = life_controller;
    }

    std::string createAgent(const AgentDef &agent_def) {
        auto id = world_.addAgentBody(agent_def.agent_body_config, life_controller_);
        auto agent = new Agent(world_.getStorage()->getAgentBody(id));
        agent->init(agent_def.agent_config);

        storage_.putAgent(id, agent);

        return id;
    }


};
