#pragma once
#include <Agent.h>
#include <thread>
#include <World.h>


class Simulation {
    World world_;
    std::unordered_map<std::string, Agent *> agents_;

    double dt_;
    int need_tps_;

    std::atomic<bool> running{};
    std::thread worker_thread_;

    void tick() {
        // сенсоры
        for (auto *agent : agents_ | std::views::values)
            agent->resolveSensors();

        // обрабатываем
        for (auto *agent : agents_ | std::views::values)
            agent->tick();
        world_.tick(dt_);
    }

public:
    explicit Simulation(const SimulationDef &def) : dt_(def.dt), need_tps_(def.need_tps) {
        // создаем препятствия
        for (auto obstacle_def : def.obstacles) {
            world_.addObstacle(obstacle_def.point, obstacle_def.rect);
        }

        // создаем агентов
        for (auto agent_def : def.init_agents) {
            auto id = world_.addAgentBody(agent_def.point, agent_def.pixels,agent_def.fov);
            auto *agent = new Agent(world_.getStorage()->getAgentBody(id));

            agent->init(agent_def, agent_def.num_of_common_neurons);

            agents_.insert({id, agent});
        }
    }

    void start() {
        if (running.load()) return;

        worker_thread_ = std::thread([&] {
            running = true;

            const auto step = std::chrono::milliseconds(1000 / need_tps_);
            auto next = std::chrono::steady_clock::now();

            while (running.load()) {
                tick();

                if (need_tps_ != -1) {
                    next += step;
                    std::this_thread::sleep_until(next);
                }
            }
        });
    }
    void stop(const bool join) {
        if (!running.load()) return;

        running = false;
        if (worker_thread_.joinable() && join)
            worker_thread_.join();
    }

    World *getWorld() {
        return &world_;
    }
    std::unordered_map<std::string, Agent *> getAgents() {
        return agents_;
    }
};
