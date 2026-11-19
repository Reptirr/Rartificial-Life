#pragma once

#include <QLabel>
#include <QStatusBar>

class StatusBar : public QStatusBar {

    QLabel *tps_{};
    QLabel *tick_{};
    QLabel *seed_{};

public slots:
    void onSeed(unsigned int seed) {
        seed_->setText("Seed: " + QString::number(seed));
    }
    void onTps(double tps) {
        tps_->setText("TPS: " + QString::number(tps, 'f', 2));
    }
    void onTick(unsigned int tick) {
        tick_->setText("Tick: " + QString::number(tick));
    }

public:
    StatusBar() {
        tps_ = new QLabel;
        seed_ = new QLabel;
        tick_ = new QLabel;

        tps_->setText("TPS: 0");
        tick_->setText("Tick: 0");
        seed_->setText("Seed: 0");

        addPermanentWidget(tps_);
        addPermanentWidget(tick_);
        addPermanentWidget(seed_);
    }
};
