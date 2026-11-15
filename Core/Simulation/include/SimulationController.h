#pragma once


#include <Model.h>
#include <SimulationBridge.h>
#include <SimulationStorage.h>
#include <string>

class World;


class SimulationController : public SimulationBridge {
    World &world_;
    SimulationStorage &storage_;

    StringConsumer deathCons;

public:
    explicit SimulationController(World &world, SimulationStorage &storage)
        : world_(world), storage_(storage) {
    }

    void setDeathCons(const StringConsumer &cons) {
        deathCons = cons;
    }

    std::string createAgent(AgentDef &agent_def);
    void createAgent(std::string parent_id) override;

    void deleteAgent(const std::string id) override {
        deathCons(id);

        auto *agent = storage_.removeAgent(id);
        delete agent;
    }
};
