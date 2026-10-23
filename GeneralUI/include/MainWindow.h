#pragma once

#include <Brain.h>
#include <BrainView.h>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Brain brain_;
    BrainView *brain_view_;

    std::thread worker_thread;

public:
    MainWindow() {
        brain_view_ = new BrainView(this, brain_.getStorage());

        setFixedSize(1000, 1000);
        brain_view_->setFixedSize(1000, 1000);

        // connect logic with ui
        brain_.setNewNeuronCallback([&](const std::string &s) { brain_view_->onNewNeuron(s); });
        brain_.setNewSynapsCallback([&](const std::string& s) { brain_view_->onNewSynaps(s); });
        brain_.setUpdateAxonCallback([&](const std::string &s) { brain_view_->onUpdateAxon(s); });
        brain_.setUpdateDendriteCallback([&](const std::string &s) {brain_view_->onUpdateDendrite(s); });

        worker_thread = std::thread([&] {
            brain_.init(200);

            while (true) {
                brain_.tact();
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
            }
        });
    }


};