#pragma once

#include <QHBoxLayout>
#include <QStackedWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

/// the abstraction over stacked_widget and toolbar
class StackedWidget : public QStackedWidget {
    Q_OBJECT

    AgentTabs *agent_tabs_{};

signals:
    void agentTabOpen(Uuid uuid);
    void agentTabClose(Uuid uuid);

public slots:
    void onAgentOpen(const Uuid& uuid, Brain *brain) const {
        agent_tabs_->onAgentOpen(uuid, brain);
    }
    void onAgentClose(const Uuid& uuid) const {
        agent_tabs_->onAgentClose(uuid);
    }

    void onStackedIndexChange(int index) {
        if (index < 0) {
            hide();
            return;
        }

        setCurrentIndex(index);
        show();
    }

public:
    explicit StackedWidget() {
        hide();

        agent_tabs_ = new AgentTabs;

        // init stacked widgets
        addWidget(agent_tabs_);

        // connects
        connect(agent_tabs_, &AgentTabs::agentTabOpen, this, [&](Uuid uuid) {
            emit agentTabOpen(uuid);
        });
        connect(agent_tabs_, &AgentTabs::agentTabClose, this, [&](Uuid uuid) {
            emit agentTabClose(uuid);
        });
    }
};