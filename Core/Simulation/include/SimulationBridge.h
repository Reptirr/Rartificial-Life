#pragma once
#include <string>

class SimulationBridge {
public:
    virtual void deleteAgent(std::string id) = 0;
    virtual void createAgent(std::string parent_id) = 0;

    virtual ~SimulationBridge() = default;
};
