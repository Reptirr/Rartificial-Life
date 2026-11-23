#pragma once
#include <string>
#include <Uuid.h>

class SimulationBridge {
public:
    virtual void deleteAgent(const Uuid &id) = 0;
    virtual void createAgent(const Uuid &parent_id) = 0;

    virtual ~SimulationBridge() = default;
};
