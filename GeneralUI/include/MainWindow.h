#pragma once

#include <AgentTabs.h>
#include <BrainView.h>
#include <GenomeRandomizer.h>
#include <Model.h>
#include <QMainWindow>
#include <QSplitter>
#include <Simulation.h>
#include <WorldView.h>
#include <MainExperiment.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

    std::unique_ptr<Simulation> simulation_;

    QSplitter *splitter_{};
    AgentTabs *agent_tabs_{};
    WorldView *world_view_{};

public:
    MainWindow() {
        auto sim_def = SimulationDef{
            .experiment = new MainExperiment,
            .need_tps = -1,
            .dt = 1/60.0
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        agent_tabs_ = new AgentTabs;

        world_view_ = new WorldView(this);
        world_view_->connectWorld(simulation_->getWorld());

        connect(world_view_, &WorldView::agentOpen, this, [&](const std::string &id) {
            auto *brain = simulation_->getAgents().at(id)->getBrain();
            agent_tabs_->onAgentOpen(id, brain);
        });
        connect(simulation_.get(), &Simulation::onAgentDeath, agent_tabs_, &AgentTabs::onAgentClose);

        agent_tabs_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        world_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        splitter_ = new QSplitter(this);
        setCentralWidget(splitter_);

        splitter_->addWidget(agent_tabs_);
        splitter_->addWidget(world_view_);


        simulation_->start();
    }


};
