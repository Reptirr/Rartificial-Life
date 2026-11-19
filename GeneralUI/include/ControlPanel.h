#pragma once
#include <ActionManager.h>
#include <QActionGroup>
#include <QToolBar>

/*
 *
 * | START/STOP |
 *
 */


class ControlPanel : public QToolBar {
    Q_OBJECT

    ActionManager *actions_{};
    QAction *simLife_{};

public slots:
    void onSimLife(bool running) {
        if (running) {
            simLife_->setText("Stop");
            simLife_->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
        } else {
            simLife_->setText("Start");
            simLife_->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        }
    }

public:
    explicit ControlPanel(ActionManager *actions, QWidget *parent = nullptr) : QToolBar(parent), actions_(actions) {
        simLife_ = actions_->simLifeAction();

        setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        addAction(simLife_);
        simLife_->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
        simLife_->setText("Start");
    }
};