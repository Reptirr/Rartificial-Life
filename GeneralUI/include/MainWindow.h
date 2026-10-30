#pragma once

#include <BrainView.h>
#include <QHBoxLayout>
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
        int num_vision_pixels = 32;

        std::vector<BrainPoint> vision_points;

        BrainPoint p{.x = -0.95, .y = -0.95};
        double step = 0.95*2 / (num_vision_pixels*3);
        for (int i = 0; i < num_vision_pixels*3; i++) {
            vision_points.push_back(p);
            p += {.y = step};
        }

        // defs
        auto agent_def = AgentDef{
            .num_of_common_neurons = 10,
            .pixels = num_vision_pixels,
            .fov = std::numbers::pi, // 180
            .vision_neurons = vision_points
        };
        auto obstacle_def = ObstacleDef{
            .point = {10, 10},
            .rect = {0, 0, 4, 4}
        };
        auto sim_def = SimulationDef{
            .init_agents = {agent_def},
            .obstacles = {obstacle_def},
            .need_tps = 100
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        // ui
        brain_view_ = new BrainView(nullptr, w, h);
        brain_view_->connectWithBrain(simulation_->getAgents().begin()->second->getBrain());
        brain_view_->setBaseSize(1000, 1000);

        world_view_ = new WorldView(simulation_->getWorld()->getStorage()->getWorldId());
        setCentralWidget(world_view_);

        // auto *main_lay = new QHBoxLayout(this);
        // main_lay->addWidget(brain_view_);
        // main_lay->addWidget(world_view_);


        simulation_->start();
    }


};
