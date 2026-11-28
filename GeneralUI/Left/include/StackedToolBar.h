#pragma once
#include <ActionManager.h>
#include <QToolBar>

class StackedToolBar : public QToolBar {
    ActionManager *action_manager_{};

public:
    explicit StackedToolBar(ActionManager *action_manager) : action_manager_(action_manager) {
        setMovable(false);
        setFloatable(false);

        addAction(action_manager_->brainWidgetOpenAction());
    }
};
