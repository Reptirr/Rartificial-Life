#pragma once

#include <AgentConfig.h>
#include <BrainView.h>
#include <GenomeRandomizer.h>
#include <Model.h>
#include <QMainWindow>
#include <QSplitter>
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
        // defs
        auto agent_def = AgentDef{
            .agent_body_config = {
                .pos = {},
                .rays = 64,
                .fov = std::numbers::pi,

                .move_dirs = 40
            },
            .agent_config = {
                .genome = GenomeRandomizer::randomizing(),
                .num_of_common_neurons = 777,
                .input_groups = {
                    NeuronGroup{
                        .num = 64*3,
                        .align = BrainAlign::Left
                    }
                },
                .output_groups = {
                    NeuronGroup{
                        .num = 40,
                        .align = BrainAlign::Top
                    }
                }
            }
        };
        // верхняя и нижняя: длинные, перекрывают углы
        auto wall_top = ObstacleDef{
            .point = {0, 15},
            .rect = {0, 0, 24, 2},
            .color = QColorConstants::Gray
        };
        auto wall_bottom = ObstacleDef{
            .point = {0, -15},
            .rect = {0, 0, 24, 2},
            .color = QColorConstants::Black
        };

        // левая и правая: помещаются между верхней и нижней
        auto wall_left = ObstacleDef{
            .point = {-15, 0},
            .rect = {0, 0, 2, 20},
            .color = QColorConstants::Red
        };
        auto wall_right = ObstacleDef{
            .point = {15, 0},
            .rect = {0, 0, 2, 20},
            .color = QColorConstants::DarkYellow
        };
        auto sim_def = SimulationDef{
            .init_agents = {agent_def},
            .obstacles = {},
            .need_tps = -1,
            .dt = 0.01
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
