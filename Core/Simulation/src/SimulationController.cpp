#include <SimulationController.h>
#include <World.h>
#include <AgentBody.h>
#include <GenomeRandomizer.h>

Uuid SimulationController::createAgent(AgentDef &agent_def) {
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

void SimulationController::createAgent(const Uuid &parent_id) {
    AgentDef def{};

    def.agent_body_config = world_.getStorage()->getAgentBody(parent_id)->getAgentBodyConfig();
    def.agent_config = storage_.getAgent(parent_id)->getConfig();

    // set pos to next to parent
    def.agent_body_config.pos = b2Body_GetPosition(world_.getStorage()->getObject(parent_id)->getBodyId());
    def.agent_body_config.pos = def.agent_body_config.pos - b2Pos{0.5, 0.5};

    // mutate
    def.agent_config.genome = GenomeRandomizer::randomize(def.agent_config.genome);

    createAgent(def);
}
