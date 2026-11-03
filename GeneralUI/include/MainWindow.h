#pragma once

#include <BrainView.h>
#include <QSplitter>
#include <QMainWindow>
#include <Simulation.h>
#include <WorldView.h>

class MainWindow : public QMainWindow {
    std::unique_ptr<Simulation> simulation_;

    BrainView *brain_view_;
    WorldView *world_view_;

    int w = 1000;
    int h = 1000;

public:
    MainWindow() {
        // logic
        int num_vision_pixels = 320;
        int num_move_dirs = 4;

        // defs
        auto agent_def = AgentDef{
            .agent_body_config = {
                .rays = 64,
                .fov = std::numbers::pi
            },
            .agent_config = {
                .genome = {
                    .proteins = {},
                    .genblocks = {
                        DefaultGenblock::get
                    }
                },
                .num_of_common_neurons = 100,
                .input_groups = {
                    NeuronGroup{
                        .num = 64*3,
                        .align = BrainAlign::Left
                    }
                }
            }
        };
        auto obstacle_def = ObstacleDef{
            .point = {15, 15},
            .rect = {0, 0, 4, 4},
            .color = QColorConstants::Black
        };
        auto obstacle_def1 = ObstacleDef{
            .point = {10, -15},
            .rect = {0, 0, 4, 8},
            .color = QColorConstants::Green
        };
        auto sim_def = SimulationDef{
            .init_agents = {agent_def},
            .obstacles = {obstacle_def1, obstacle_def},
            .need_tps = -1,
            .dt = 0.1
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        // ui
        brain_view_ = new BrainView(nullptr, w, h);
        brain_view_->connectWithBrain(simulation_->getAgents().begin()->second->getBrain());

        world_view_ = new WorldView(this);
        world_view_->connectWorld(simulation_->getWorld());

        auto *splitter = new QSplitter(this);
        setCentralWidget(splitter);

        splitter->addWidget(brain_view_);
        splitter->addWidget(world_view_);


        simulation_->start();
    }


};
