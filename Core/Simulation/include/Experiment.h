#pragma once

class World;
class Simulation;
class SimulationController;
class WorldController;
class SimulationStorage;

struct TickContext {
    int tick_i{};

    Simulation *simulation{};
    World *world{};

    SimulationController *sim_controller{};

    SimulationStorage *sim_storage{};
};

// interface
class Experiment {
public:
    virtual void preFirstTick(TickContext &ctx) {}
    virtual void onTick(TickContext &ctx) = 0;

    virtual ~Experiment() = default;
};