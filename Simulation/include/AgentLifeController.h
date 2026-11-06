#pragma once
#include <AgentBodyConfig.h>
#include <GenomeRandomizer.h>
#include <SimulationStorage.h>

struct toBirth {
    std::string id;
    AgentBodyConfig conf;
};

class AgentLifeController {
    SimulationController &controller_;
    SimulationStorage &storage_;

    std::queue<toBirth> births_;

public:
    AgentLifeController(SimulationController &controller, SimulationStorage &storage)
        : controller_(controller),
          storage_(storage) {
    }

    void requestBirth(const std::string &parent_id, AgentBodyConfig conf) {
        births_.push({parent_id, conf});
    }

    void applyBirths() {
        while (!births_.empty()) {
            auto to_birth = births_.front();
            births_.pop();

            auto *parent = storage_.getAgent(to_birth.id);
            auto agent_body_conf = to_birth.conf;

            if (!parent)
                continue; // родитель мертв

            auto agent_config = parent->getConfig();
            agent_config.genome = GenomeRandomizer::randomize(agent_config.genome);

            AgentDef def{
                .agent_body_config = agent_body_conf,
                .agent_config = agent_config
            };

            controller_.createAgent(def);
        }
    }
};
