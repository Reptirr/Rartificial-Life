#pragma once


#include <AgentBody.h>
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

    std::string createAgent(AgentDef &agent_def) {
        auto id = world_.addAgentBody(agent_def.agent_body_config, life_controller_);
        auto *agentbody = world_.getStorage()->getAgentBody(id);

        // set agent config groups
        agent_def.agent_config.input_groups = agentbody->getInputNeuronGroups();
        agent_def.agent_config.output_groups = agentbody->getOutputNeuronGroups();

        auto agent = new Agent(world_.getStorage()->getAgentBody(id));
        agent->init(agent_def.agent_config);

        storage_.putAgent(id, agent);

        return id;
    }


};
