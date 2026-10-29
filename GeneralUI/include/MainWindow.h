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

        brain_view_->connectWithBrain(brain_);

        worker_thread = std::thread([&] {
            const auto genome = Genome{
                .proteins = {
                    {
                        .point = {0, 0},
                        .dist = 0.03,
                        .genblock_i = 0
                    }
                },
                .genblocks = {
                    DefaultGenblock::get,
                    {
                        {Gen::SYNAPS_W_INIT, "10"}
                    }
                }
            };

            agent_.init(genome, 1000);

            BrainPoint input_p{-0.4, -0.4};
            for (int i = 0; i < 800; i++) {
                agent_.createInputNeuron(input_p, 0.5);

                input_p += BrainPoint{0.0, 0.0003};
            }

            BrainPoint output_p{0.4, 0.4};
            for (int i = 0; i < 50; i++) {
                agent_.createOutputNeuron(output_p);

                output_p += BrainPoint{0.0, 0.0003};
            }

            // set tps to 100
            const auto step = std::chrono::milliseconds(10);
            auto next = std::chrono::steady_clock::now();

            while (true) {
                agent_.tact();

                next += step;
                std::this_thread::sleep_until(next);
            }
        });
    }


};
