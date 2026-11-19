#pragma once
#include <qaction.h>
#include <QApplication>

#define ACTION(name) \
        QAction *name##_{}; \
    public: \
        QAction *name##Action() const { return name##_; } \
    private:

inline void action_connect(QAction *act, std::function<void(bool)> func) {
    QObject::connect(act, &QAction::toggled, func);
}

class ActionManager : public QObject {
    Q_OBJECT

    ACTION(simLife)

signals:
    void simLifeTrigger();

public:
    explicit ActionManager(QObject *parent) : QObject(parent) {
        simLife_ = new QAction("Start");

        connect(simLife_, &QAction::triggered, this, &ActionManager::simLifeTrigger);
    }
};



#undef ACTION
#undef ACTION_CONNECT