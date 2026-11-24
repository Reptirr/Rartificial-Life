#pragma once
#include <BrainView.h>
#include <QObject>
#include <SimulationController.h>
#include <SimulationRunner.h>
#include <SimulationStorage.h>
#include <World.h>
#include <WorldView.h>

class Simulation : public QObject {
    Q_OBJECT

    World world_;
    SimulationStorage storage_;
    SimulationController controller_{world_, storage_};
    SimulationRunner *runner_{};

    SimulationDef def_;


signals:
    void agentBirth(Uuid id);
    void agentDeath(Uuid id);
    void simLife(bool running);

    void tpsValue(double tps);
    void foodValue(int num);

    void newTick(unsigned int tick);
    void seedValue(unsigned int seed);

public slots:
    void onSimLifeToggle() {
        runner_->toggle();

        emit simLife(runner_->running());
    }

public:
    explicit Simulation(const SimulationDef &def) : world_(def.dt), def_(def) {
        controller_.setDeathCons([&](const Uuid &id) { emit agentDeath(id); });

        world_.init(&controller_);

        runner_ = new SimulationRunner(this, &controller_, &storage_, def, [&](double tps) { emit tpsValue(tps); },
                                       [&](int tick) { emit newTick(tick); });
    }

    void connectUIBrain(BrainView *view, const Uuid &agent) const {
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
    const std::unordered_map<Uuid, Agent *, UuidHash>& getAgents() {
        return storage_.agents();
    }

    ~Simulation() override {
        delete def_.experiment;
    }
};
