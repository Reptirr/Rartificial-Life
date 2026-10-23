#pragma once

#include <Brain.h>
#include <BrainView.h>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Brain brain_;
    BrainView *brain_view_;

    std::thread worker_thread;

    int w = 1000;
    int h = 1000;

public:
    MainWindow() {
        brain_view_ = new BrainView(this, brain_.getStorage(), w, h);

        setFixedSize(w, h);

        // connect logic with ui
        brain_.setNewNeuronCallback([&](const std::string &s) { brain_view_->onNewNeuron(s); });
        brain_.setNewSynapsCallback([&](const std::string& s) { brain_view_->onNewSynaps(s); });
        brain_.setUpdateAxonCallback([&](const std::string &s) { brain_view_->onUpdateAxon(s); });
        brain_.setUpdateDendriteCallback([&](const std::string &s) {brain_view_->onUpdateDendrite(s); });

        worker_thread = std::thread([&] {
            brain_.init(200);

            BrainPoint input_p{-0.4, -0.4};
            for (int i = 0; i < 5; i++) {
                brain_.createInputNeuron(input_p);

                input_p += BrainPoint{0.0, 0.03};
            }

            BrainPoint output_p{0.4, 0.4};
            for (int i = 0; i < 5; i++) {
                brain_.createOutputNeuron(output_p);

                output_p += BrainPoint{0.0, 0.03};
            }

            while (true) {
                brain_.tact();
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
            }
        });
    }


};