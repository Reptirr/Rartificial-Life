#pragma once
#include <Agent.h>
#include <BrainView.h>
#include <SimulationController.h>
#include <SimulationStorage.h>
#include <thread>
#include <World.h>
#include <WorldView.h>


class Simulation : public QObject {
    Q_OBJECT

    World world_;
    SimulationStorage storage_;
    SimulationController controller_{world_, storage_};

    SimulationDef def_;

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

signals:
    void onAgentDeath(std::string id);

public:
    explicit Simulation(const SimulationDef &def) : world_(def.dt), def_(def) {
        controller_.setDeathCons([&](const std::string &id) { emit onAgentDeath(id); });

        world_.init(&controller_);
    }

    void start() {
        if (running.load()) return;

        worker_thread_ = std::thread([&] {
            running = true;

            const auto step = std::chrono::milliseconds(1000 / def_.need_tps);
            auto next = std::chrono::steady_clock::now();

            auto ctx = TickContext{
                .tick_i = 0,
                .simulation = this,
                .world = &world_,

                .sim_controller = &controller_,
                .sim_storage = &storage_
            };

            while (running.load()) {
                def_.experiment->onTick(ctx);
                ctx.tick_i++;
                tick();

                if (def_.need_tps != -1) {
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

    World *getWorld() {
        return &world_;
    }
    const std::unordered_map<std::string, Agent *>& getAgents() {
        return storage_.agents();
    }

    ~Simulation() override {
        stop(true);
        delete def_.experiment;
    }
};
