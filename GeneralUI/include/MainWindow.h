#pragma once

#include <AgentConfig.h>
#include <AgentTabs.h>
#include <BrainView.h>
#include <GenomeRandomizer.h>
#include <Model.h>
#include <QMainWindow>
#include <QSplitter>
#include <Simulation.h>
#include <WorldView.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

    std::unique_ptr<Simulation> simulation_;

    QSplitter *splitter_{};
    AgentTabs *agent_tabs_{};
    WorldView *world_view_{};

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
        auto sim_def = SimulationDef{
            .init_agents = {agent_def},
            .obstacles = {},
            .need_tps = -1,
            .dt = 0.01
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        agent_tabs_ = new AgentTabs;

        world_view_ = new WorldView(this);
        world_view_->connectWorld(simulation_->getWorld());

        connect(world_view_, &WorldView::agentOpen, this, [&](const std::string &id) {
            auto *brain = simulation_->getAgents().at(id)->getBrain();
            agent_tabs_->onAgentOpen(id, brain);
        });

        splitter_ = new QSplitter(this);
        setCentralWidget(splitter_);

        splitter_->addWidget(agent_tabs_);
        splitter_->addWidget(world_view_);


        simulation_->start();
    }


};
