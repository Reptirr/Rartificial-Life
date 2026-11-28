#pragma once
#include <qaction.h>
#include <QActionGroup>
#include <QApplication>

#define ACTION(name) \
        QAction *name##_{}; \
    public: \
        QAction *name##Action() const { return name##_; } \
    private:

inline void action_connect(QAction *act, std::function<void(bool)> func) {
    QObject::connect(act, &QAction::toggled, /*тут*/func);
}

class ActionManager : public QObject {
    Q_OBJECT

    ACTION(simLife)
    ACTION(brainWidgetOpen)

    QActionGroup *stacked_group_{};

signals:
    void simLifeTrigger();
    void stackedChangeIndex(int index);

public:
    explicit ActionManager(QObject *parent) : QObject(parent) {
        simLife_ = new QAction("Start");
        brainWidgetOpen_ = new QAction(QIcon(":/icons/brain.svg"), "Brain");

        brainWidgetOpen_->setData(0); // stack index
        brainWidgetOpen_->setCheckable(true);

        stacked_group_ = new QActionGroup(this);
        stacked_group_->addAction(brainWidgetOpen_);
        stacked_group_->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

        // shortcuts
        simLife_->setShortcut(Qt::Key_Space);

        brainWidgetOpen_->setShortcut(Qt::Key_1);

        connect(simLife_, &QAction::triggered, this, &ActionManager::simLifeTrigger);
        connect(stacked_group_, &QActionGroup::triggered, [&](QAction *action) {
            emit stackedChangeIndex(
                action->isChecked() ? action->data().toInt()
                                    : -1
            );
        });
    }
};



#undef ACTION
#undef ACTION_CONNECT