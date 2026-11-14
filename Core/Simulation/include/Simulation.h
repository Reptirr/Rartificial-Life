#pragma once
#include <Agent.h>
#include <SimulationController.h>
#include <SimulationStorage.h>
#include <thread>
#include <World.h>
#include <WorldView.h>


class Simulation {
    World world_;
    SimulationStorage storage_;
    SimulationController controller_{world_, storage_};

    int need_tps_;

    std::atomic<bool> running{};
    std::thread worker_thread_;

    void tick() {
        // сенсоры
        for (auto *agent : storage_.agents() | std::views::values)
            agent->handleSensors();

        // обрабатываем
        for (auto *agent : storage_.agents() | std::views::values)
            agent->tick();
        world_.tick();

        // движения
        for (auto *agent : storage_.agents() | std::views::values)
            agent->handleOutputs();
    }

public:
    explicit Simulation(const SimulationDef &def) : world_(def.dt), need_tps_(def.need_tps) {
        world_.init(&controller_);

        // создаем препятствия
        for (auto obstacle_def : def.obstacles) {
            world_.addObstacle(obstacle_def.point, obstacle_def.rect, obstacle_def.color);
        }

        // создаем агентов
        for (auto agent_def : def.init_agents) {
            controller_.createAgent(agent_def);
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

                auto food_num = world_.getStorage()->getFood().size();

                if (food_num < 10000) {
                    for (int i = 0; i < 10000 - food_num; i++) {
                        QPointF pos = {
                            Rng::randReal(-100, 100),
                            Rng::randReal(-100, 100)
                        };

                        world_.addFood(pos);
                    }

                }

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

    void connectUIBrain(BrainView *view, const std::string &agent) const {
        auto *brain = storage_.getAgent(agent)->getBrain();

        view->connectWithBrain(brain);
    }
    void connectUIWorld(WorldView *view) {
        view->connectWorld(&world_);
    }

    void setOnAgentDeathConsumer(const StringConsumer &cons) {
        controller_.setOnAgentDeathConsumer(cons);
    }

    World *getWorld() {
        return &world_;
    }
    const std::unordered_map<std::string, Agent *>& getAgents() {
        return storage_.agents();
    }

    ~Simulation() {
        stop(true);
    }
};
