#pragma once
#include <BrainView.h>
#include <qtabwidget.h>
#include <Uuid.h>

class AgentTabs : public QTabWidget {
    std::unordered_map<Uuid, BrainView *> views_;

    void closeTabAt(int index) {
        if (index < 0) return;
        QWidget *w = widget(index);
        removeTab(index);                      // убирает вкладку, но не виджет
        std::erase_if(views_, [w](const auto &p) { return p.second == w; });
        delete w;  // удаляем сами
    }

public slots:
    void onAgentClose(const Uuid &id) {
        if (!views_.contains(id)) return;

        auto *view = views_.at(id);
        auto index = indexOf(view);

        closeTabAt(index);
    }

    void onAgentOpen(const Uuid &id, Brain *brain) {
        if (views_.contains(id)) {
            return;
        }

        auto *view = new BrainView(this);
        view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        view->connectWithBrain(brain);
        views_.insert({id, view});

        addTab(view, "brain");
    }

public:
    AgentTabs() {
        connect(this, &QTabWidget::tabCloseRequested, this, &AgentTabs::closeTabAt);

        setTabsClosable(true);
        setMovable(true);
    }
};
