#include <SimulationController.h>
#include <World.h>
#include <AgentBody.h>

std::string SimulationController::createAgent(AgentDef &agent_def) {
    auto id = world_.addAgentBody(agent_def.agent_body_config);
    auto *agentbody = world_.getStorage()->getAgentBody(id);

    // set agent config groups
    agent_def.agent_config.input_groups = agentbody->getInputNeuronGroups();
    agent_def.agent_config.output_groups = agentbody->getOutputNeuronGroups();

    auto agent = new Agent(world_.getStorage()->getAgentBody(id));
    agent->init(agent_def.agent_config);

    storage_.putAgent(id, agent);

    return id;
}
