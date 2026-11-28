#pragma once
#include <Model.h>
#include <thread>

class SimulationRunner {
    using Clock = std::chrono::steady_clock;

    SimulationDef def_;

    World *world_;
    SimulationStorage *storage_{};
    SimulationController *controller_{};

    std::thread worker_thread_;
    std::atomic<bool> running_{};
    std::atomic<int> tick_i{};
    TickContext ctx_;

    std::function<void(double)> tps_listener_;
    std::function<void(int)> tick_listener_;

    std::atomic<bool> tps_measuring_{};
    int previous_measure_tick_{};
    Clock::time_point previous_measure_time_;
    std::thread tps_timer_;

    void tick() const;

public:
    SimulationRunner(Simulation *simulation, SimulationController *controller, SimulationStorage *storage,
                     const SimulationDef &def, const std::function<void(double)> &tps_listener,
                     const std::function<void(int)> &tick_listener);

    bool running() const;

    void toggle();
    void start();
    void stop(bool join = true);

    ~SimulationRunner() {
        stop(true); // worker_thread is 0

        tps_measuring_ = false;
        if (tps_timer_.joinable())
            tps_timer_.join();
    }
};
