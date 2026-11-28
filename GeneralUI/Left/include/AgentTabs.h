#pragma once
#include <BrainView.h>
#include <qtabwidget.h>
#include <Uuid.h>

class AgentTabs : public QTabWidget {
    Q_OBJECT

    std::unordered_map<Uuid, BrainView *> views_;

    void closeTabAt(int index) {
        if (index < 0) return;
        QWidget *w = widget(index);
        removeTab(index);                      // убирает вкладку, но не виджет
        std::erase_if(views_, [&](const auto &p) {
            return p.second == w;
        });
        delete w;  // удаляем сами
    }

    Uuid prev_view_;

signals:
    void agentTabOpen(Uuid id);
    void agentTabClose(Uuid id);

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

        connect(this, &QTabWidget::currentChanged, [&](int index) {
            if (!prev_view_.empty()) emit agentTabClose(prev_view_); // закрываем предыдущий
            if (index != -1) {
                auto *view = static_cast<BrainView *>(widget(index));
                const auto it = std::ranges::find_if(views_, [&](const auto &p) {
                    return p.second == view;
                });
                if (it == views_.end()) return;

                emit agentTabOpen(it->first);
                prev_view_ = it->first;
            } else {
                prev_view_ = {};
            }
        });

        setTabsClosable(true);
        setMovable(true);
    }
};
