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

        std::vector<BrainPoint> vision_points;

        BrainPoint p{.x = -0.95, .y = -0.95};
        double step = 0.95*2 / (num_vision_pixels*3);
        for (int i = 0; i < num_vision_pixels*3; i++) {
            vision_points.push_back(p);
            p += {.y = step};
        }

        // defs
        auto agent_def = AgentDef{
            .num_of_common_neurons = 100,
            .pixels = num_vision_pixels,
            .fov = std::numbers::pi, // 180
            .vision_neurons = vision_points
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
            .need_tps = 40
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        // ui
        brain_view_ = new BrainView(nullptr, w, h);
        brain_view_->connectWithBrain(simulation_->getAgents().begin()->second->getBrain());

        world_view_ = new WorldView(simulation_->getWorld()->getStorage()->getWorldId());

        auto *splitter = new QSplitter(this);
        setCentralWidget(splitter);

        splitter->addWidget(brain_view_);
        splitter->addWidget(world_view_);


        simulation_->start();
    }


};
