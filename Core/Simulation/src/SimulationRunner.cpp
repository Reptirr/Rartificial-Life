#include <Simulation.h>
#include <SimulationRunner.h>
#include <SimulationStorage.h>

void SimulationRunner::tick() const {
    // сенсоры
    for (auto *agent : storage_->agents() | std::views::values)
        agent->handleSensors();

    // обрабатываем
    for (auto *agent : storage_->agents() | std::views::values)
        agent->tick();
    world_->tick();

    // движения
    for (auto *agent : storage_->agents() | std::views::values)
        agent->handleOutputs();
}

SimulationRunner::SimulationRunner(Simulation *simulation, SimulationController *controller, SimulationStorage *storage,
    const SimulationDef &def, const std::function<void(double)> &tps_listener, const std::function<void(int)> &tick_listener)
    : def_(def), world_(simulation->getWorld()), storage_(storage), controller_(controller), tps_listener_(tps_listener), tick_listener_(tick_listener) {
    ctx_.sim_controller = controller_;
    ctx_.sim_storage = storage_;
    ctx_.simulation = simulation;
    ctx_.tick_i = 0;
    ctx_.world = simulation->getWorld();

    def_.experiment->preFirstTick(ctx_);

    tps_measuring_ = true;
    tps_timer_ = std::thread([&] {
        while (tps_measuring_.load()) {
            auto tick_delta = tick_i - previous_measure_tick_;
            auto time_delta = Clock::now() - previous_measure_time_;
            double time_delta_s = std::chrono::duration<double>(time_delta).count();

            auto tps = tick_delta / time_delta_s;

            tps_listener_(tps);

            previous_measure_tick_ = tick_i;
            previous_measure_time_ = Clock::now();

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    });
}

bool SimulationRunner::running() const { return running_.load(); }

void SimulationRunner::toggle() {
    if (running_.load()) {
        stop();
    } else {
        start();
    }
}

void SimulationRunner::start() {
    if (running_.load()) return;

    running_ = true;
    previous_measure_time_ = std::chrono::steady_clock::now();
    worker_thread_ = std::thread([&] {
        const auto step = std::chrono::milliseconds(1000 / def_.need_tps);
        auto next = std::chrono::steady_clock::now();

        while (running_.load()) {
            def_.experiment->onTick(ctx_);
            ctx_.tick_i++;
            ++tick_i;
            tick();
            tick_listener_(tick_i);

            if (def_.need_tps != -1) {
                next += step;
                std::this_thread::sleep_until(next);
            }
        }
    });
}

void SimulationRunner::stop(const bool join) {
    if (!running_.load()) return;

    running_ = false;
    if (worker_thread_.joinable() && join)
        worker_thread_.join();
}
