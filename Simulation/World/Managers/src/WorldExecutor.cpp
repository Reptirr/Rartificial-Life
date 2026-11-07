#include <AgentBody.h>
#include <WorldExecutor.h>

void WorldExecutor::handleCmdFabric(const WorldCommand &cmd) {
    auto &data = cmd.data;

    switch (cmd.type) {
        case WorldCommandType::NONE:
            break;

        case WorldCommandType::AGENT_BODY_EAT_FOOD: {
            auto agentbody_id = data.at("agentbody");
            auto *agentbody = storage_->getAgentBody(agentbody_id);

            auto food_id = data.at("food");

            controller_->remove(food_id);
            agentbody->onEat();

            return;
        }

        case WorldCommandType::AGENT_BODY_MOVE: {
            auto agentbody_id = data.at("agentbody");

            auto vec_x = std::stod(data.at("vec_x"));
            auto vec_y = std::stod(data.at("vec_y"));

            controller_->setVel(agentbody_id, {(float)vec_x, (float)vec_y});
        }
    }
}
