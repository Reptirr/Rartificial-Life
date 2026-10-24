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
        brain_.setUpdateCallback([&](const std::string &s) { brain_view_->onUpdate(s); });

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

            // set tps to 100
            auto step = std::chrono::milliseconds(10);
            auto next = std::chrono::steady_clock::now();

            while (true) {
                brain_.tact();
                next += step;
                std::this_thread::sleep_until(next);
            }
        });
    }


};