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
    std::chrono::steady_clock::time_point pre_resolve_tps_time{};

    std::atomic<unsigned int> tick_i{};
    std::thread worker_thread_;
    TickContext ctx_;

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
    void simLife(bool running);

    void newTick(unsigned int tick);
    void tpsValue(double tps);
    void seedValue(unsigned int seed);

public slots:
    void onSimLifeToggle() {
        if (running) stop(true);
        else start();

        emit simLife(running.load()); // уведомляем о изменении состояния
    }

public:
    explicit Simulation(const SimulationDef &def) : world_(def.dt), def_(def) {
        controller_.setDeathCons([&](const std::string &id) { emit onAgentDeath(id); });

        world_.init(&controller_);

        ctx_ = TickContext{
            .tick_i = -1,
            .simulation = this,
            .world = &world_,

            .sim_controller = &controller_,
            .sim_storage = &storage_
        };

        def_.experiment->preFirstTick(ctx_);

        auto *timer = new QTimer(this);

        connect(timer, &QTimer::timeout, [&] {
            auto elapsed = std::chrono::duration<double>(
                std::chrono::steady_clock::now() - pre_resolve_tps_time
            ).count();
            auto tps_s = tick_i.load() / elapsed;

            emit tpsValue(tps_s);

            tick_i = 0;
            pre_resolve_tps_time = std::chrono::steady_clock::now();
        });
        timer->start(250);
    }

    void start() {
        if (running.load()) return;

        running = true;
        pre_resolve_tps_time = std::chrono::steady_clock::now();
        worker_thread_ = std::thread([&] {
            const auto step = std::chrono::milliseconds(1000 / def_.need_tps);
            auto next = std::chrono::steady_clock::now();

            auto tps_time = std::chrono::steady_clock::now();

            while (running.load()) {
                def_.experiment->onTick(ctx_);
                ctx_.tick_i++;
                ++tick_i;
                emit newTick(ctx_.tick_i);
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

    void setSeed(const unsigned int seed) {
        Rng::seed(seed);
        emit seedValue(seed);
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
