#pragma once

#include <ActionManager.h>
#include <AgentTabs.h>
#include <BrainView.h>
#include <ControlPanel.h>
#include <MainExperiment.h>
#include <Model.h>
#include <QMainWindow>
#include <QSplitter>
#include <Simulation.h>
#include <StackedToolBar.h>
#include <StackedWidget.h>
#include <StatusBar.h>
#include <WorldView.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

    std::unique_ptr<Simulation> simulation_;

    ActionManager *action_manager_{};

    // up
    ControlPanel *control_panel_{};

    // bottom
    StatusBar *status_bar_{};

    // center
    StackedToolBar *stacked_tool_bar_{};
    StackedWidget *stacked_widget_{};
    QSplitter *splitter_{};
    WorldView *world_view_{};


    void connects() {
        // WORLD_VIEW/SIMULATION -> STACKED_WIDGET
        connect(world_view_, &WorldView::agentOpen, this, [&](const Uuid &id) {
            auto *brain = simulation_->getAgents().at(id)->getBrain();
            stacked_widget_->onAgentOpen(id, brain);
        });
        connect(simulation_.get(), &Simulation::agentDeath, stacked_widget_, &StackedWidget::onAgentClose);

        // ACTIONS -> SIM (управление)
        connect(action_manager_, &ActionManager::simLifeTrigger, simulation_.get(), &Simulation::onSimLifeToggle);

        // SIM -> CONTROL_PANEL (изменение ui)
        connect(simulation_.get(), &Simulation::simLife, control_panel_, &ControlPanel::onSimLife);

        // SIM -> STATUS_BAR (изменение ui)
        connect(simulation_.get(), &Simulation::newTick, status_bar_, &StatusBar::onTick);
        connect(simulation_.get(), &Simulation::tpsValue, status_bar_, &StatusBar::onTps);
        connect(simulation_.get(), &Simulation::seedValue, status_bar_, &StatusBar::onSeed);

        // ACTIONS -> STACKED_WIDGET (изменение индекса)
        connect(action_manager_, &ActionManager::stackedChangeIndex, stacked_widget_, &StackedWidget::onStackedIndexChange);

        // STACKED_WIDGET(AGENTS_TABS) -> WORLD_VIEW
        connect(stacked_widget_, &StackedWidget::agentTabOpen, world_view_, &WorldView::onAgentTabOpen);
        connect(stacked_widget_, &StackedWidget::agentTabClose, world_view_, &WorldView::onAgentTabClose);
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

        world_view_ = new WorldView(this);
        world_view_->connectWorld(simulation_->getWorld());
        world_view_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // ---- TOP TOOLBAR ----
        control_panel_ = new ControlPanel(action_manager_);
        addToolBar(Qt::ToolBarArea::TopToolBarArea, control_panel_);

        // ---- CENTER -----
        stacked_tool_bar_ = new StackedToolBar(action_manager_);
        addToolBar(Qt::LeftToolBarArea, stacked_tool_bar_);

        stacked_widget_ = new StackedWidget;
        stacked_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        splitter_ = new QSplitter(this);
        splitter_->addWidget(stacked_widget_);
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
