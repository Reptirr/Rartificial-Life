#pragma once

#include <Brain.h>
#include <BrainView.h>
#include <QMainWindow>
#include <Agent/include/Agent.h>

class MainWindow : public QMainWindow {
    Agent agent_;
    Brain *brain_;
    BrainView *brain_view_;

    std::thread worker_thread;

    int w = 1000;
    int h = 1000;

public:
    MainWindow() {
        brain_ = agent_.getBrain();
        brain_view_ = new BrainView(this, brain_->getStorage(), w, h);

        setFixedSize(w, h);

        // connect logic with ui
        brain_->setNewNeuronCallback([&](const std::string &s) { brain_view_->onNewNeuron(s); });
        brain_->setNewSynapsCallback([&](const std::string& s) { brain_view_->onNewSynaps(s); });
        brain_->setUpdateCallback([&](const std::string &s) { brain_view_->onUpdate(s); });

        agent_.createCommonNeurons(100);

        worker_thread = std::thread([&] {
            BrainPoint input_p{-0.4, -0.4};
            for (int i = 0; i < 5; i++) {
                agent_.createInputNeuron(input_p, 0.5);

                input_p += BrainPoint{0.0, 0.03};
            }

            std::vector<std::string> outputs;

            BrainPoint output_p{0.4, 0.4};
            for (int i = 0; i < 5; i++) {
                outputs.push_back(agent_.createOutputNeuron(output_p));

                output_p += BrainPoint{0.0, 0.03};
            }

            // set tps to 100
            auto step = std::chrono::milliseconds(10);
            auto next = std::chrono::steady_clock::now();

            while (true) {
                agent_.tact();

                for (int i = 0; i < outputs.size(); i++) {
                    const auto &id = outputs.at(i);

                    // qDebug() << "O" << i << ": " << brain_->getStorage().get<Neuron>(id)->getSignal();
                }

                next += step;
                std::this_thread::sleep_until(next);
            }
        });
    }


};
