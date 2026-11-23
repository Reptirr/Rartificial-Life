#pragma once


#include <Model.h>
#include <SimulationBridge.h>
#include <SimulationStorage.h>
#include <string>
#include <Uuid.h>

class World;


class SimulationController : public SimulationBridge {
    World &world_;
    SimulationStorage &storage_;

    IdConsumer deathCons;

public:
    explicit SimulationController(World &world, SimulationStorage &storage)
        : world_(world), storage_(storage) {
    }

    void setDeathCons(const IdConsumer &cons) {
        deathCons = cons;
    }

    Uuid createAgent(AgentDef &agent_def);
    void createAgent(const Uuid &parent_id) override;

    void deleteAgent(const Uuid &id) override {
        deathCons(id);

        auto *agent = storage_.removeAgent(id);
        delete agent;
    }
};
