#pragma once

#include <Brain.h>
#include <BrainView.h>
#include <QMainWindow>
#include <World.h>
#include <WorldView.h>
#include <Agent/include/Agent.h>

class MainWindow : public QMainWindow {
    Agent *agent_;
    Brain *brain_;
    BrainView *brain_view_;

    World *world_;
    WorldView *world_view_;

    std::thread worker_thread;

    Genome genome = {
        .proteins = {
            {
                .point = {0, 0},
                .dist = 0.03,
                .genblock_i = 0
            }
        },
        .genblocks = {
            DefaultGenblock::get,
            {
                                {Gen::SYNAPS_W_INIT, "10"}
            }
        }
    };

    AgentConfig conf = {
        .genome = genome,
        .eyes = 1,
        .pixels_in_eye = 12,
        .first_input_neuron_pos = {.x = -0.8, .y = -0.95},
        .space_between_input_neurons_ = {0.00, 0.04}
    };

    int w = 1000;
    int h = 1000;

public:
    MainWindow() {
        world_ = new World();
        world_view_ = new WorldView(world_->getStorage()->getWorldId());


        const auto agent_id = world_->addAgentBody({}, conf.pixels_in_eye);

        agent_ = new Agent(world_->getStorage()->getAgentBody(agent_id));

        brain_ = agent_->getBrain();
        brain_view_ = new BrainView(this, brain_->getStorage(), w, h);

        setFixedSize(w, h);

        brain_view_->connectWithBrain(brain_);

        worker_thread = std::thread([&] {
            agent_->init(conf, 10);

            // set tps to 100
            const auto step = std::chrono::milliseconds(10);
            auto next = std::chrono::steady_clock::now();

            while (true) {
                agent_->resolveSensors();
                agent_->tact();
                world_->tact(0.005); // 5мс

                next += step;
                std::this_thread::sleep_until(next);
            }
        });
    }


};
