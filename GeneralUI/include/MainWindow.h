#pragma once

#include <ActionManager.h>
#include <AgentTabs.h>
#include <BrainView.h>
#include <ControlPanel.h>
#include <GenomeRandomizer.h>
#include <MainExperiment.h>
#include <Model.h>
#include <QMainWindow>
#include <QSplitter>
#include <Simulation.h>
#include <StatusBar.h>
#include <WorldView.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

    std::unique_ptr<Simulation> simulation_;

    ActionManager *action_manager_{};
    ControlPanel *control_panel_{};
    StatusBar *status_bar_{};

    QSplitter *splitter_{};
    AgentTabs *agent_tabs_{};
    WorldView *world_view_{};


    void connects() {
        // WORLD_VIEW/SIMULATION -> AGENT_TABS
        connect(world_view_, &WorldView::agentOpen, this, [&](const Uuid &id) {
            auto *brain = simulation_->getAgents().at(id)->getBrain();
            agent_tabs_->onAgentOpen(id, brain);
        });
        connect(simulation_.get(), &Simulation::agentDeath, agent_tabs_, &AgentTabs::onAgentClose);

        // ACTIONS -> SIM (управление)
        connect(action_manager_, &ActionManager::simLifeTrigger, simulation_.get(), &Simulation::onSimLifeToggle);

        // SIM -> CONTROL_PANEL (изменение ui)
        connect(simulation_.get(), &Simulation::simLife, control_panel_, &ControlPanel::onSimLife);

        // SIM -> STATUS_BAR (изменение ui)
        connect(simulation_.get(), &Simulation::newTick, status_bar_, &StatusBar::onTick);
        connect(simulation_.get(), &Simulation::tpsValue, status_bar_, &StatusBar::onTps);
        connect(simulation_.get(), &Simulation::seedValue, status_bar_, &StatusBar::onSeed);
    }

public:
    MainWindow() {
        action_manager_ = new ActionManager(this);

        auto sim_def = SimulationDef{
            .experiment = new MainExperiment,
            .need_tps = -1,
            .dt = 1/60.0
        };

        simulation_ = std::make_unique<Simulation>(sim_def);

        agent_tabs_ = new AgentTabs;
        agent_tabs_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        world_view_ = new WorldView(this);
        world_view_->connectWorld(simulation_->getWorld());
        world_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // ---- TOP TOOLBAR ----
        control_panel_ = new ControlPanel(action_manager_);
        addToolBar(Qt::ToolBarArea::TopToolBarArea, control_panel_);

        // ---- CENTER -----
        splitter_ = new QSplitter(this);
        splitter_->addWidget(agent_tabs_);
        splitter_->addWidget(world_view_);

        // ---- BOTTOM ----
        status_bar_ = new StatusBar;
        setStatusBar(status_bar_);

        setCentralWidget(splitter_);

        connects();

        // seed
        simulation_->setSeed(std::random_device()());
    }


};
