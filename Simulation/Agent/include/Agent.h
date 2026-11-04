#pragma once

#include <AgentBody.h>
#include <AgentConfig.h>
#include <Model.h>

class Agent {
    Brain brain_;
    AgentBody *agent_body_;

    // --------------
    // INPUT
    // --------------

    // текущие частоты спайков на входные нейроны.
    // обозначает раз в сколько таков делать спайк
    std::unordered_map<std::string, int> input_freqs;

    // счетчик, показывающий через сколько делать спайк. per tact
    // при изменении частоты, он делает спайк а затем начинает с новой частотой
    std::unordered_map<std::string, int> input_counters;

    std::vector<std::string> input_neurons;
    std::vector<std::string> output_neurons;



    int convertIntensityToFreq(double intensity) {
        /*
         * если интенсивность = 1, то freq = 5
         * если интенсивность = 0.5, то freq = 10
         * если интенсивность = 0, то freq = 0
         */

        constexpr double MIN_PERIOD = 5.0; // refractor min
        if (intensity <= 0.0) return 0;
        if (intensity > 1.0) intensity = 1.0;
        return static_cast<int>(std::round(MIN_PERIOD / intensity));
    }
    void setInputNeuronIntensity(const std::string &id, const double intensity) {
        my_assert(input_freqs.contains(id));
        my_assert(input_counters.contains(id));

        const int freq = convertIntensityToFreq(intensity);

        input_freqs.at(id) = freq;

        auto *neuron = brain_.getStorage().get<Neuron>(id);
        neuron->setIntensity(intensity);
    }
    bool getNeuronSpiked(const std::string &id) {
        auto *neuron = brain_.getStorage().get<Neuron>(id);
        auto isSpiked = neuron->isSpiked();

        return isSpiked;
    }

    std::string createInputNeuron(BrainPoint p, double init_intensity) {
        auto res = brain_.createInputNeuron(p);

        // init freq & counter
        int freq = convertIntensityToFreq(init_intensity);

        input_freqs.insert({res.at("neuron"), freq});
        input_counters.insert({res.at("neuron"), 1});

        return res.at("neuron");
    }
    std::string createOutputNeuron(BrainPoint p) {
        return brain_.createOutputNeuron(p).at("neuron");
    }

    // utils
    void align_helper(BrainAlign align, int num, BrainPoint &first_p, BrainPoint &step_p) {
        if (num <= 0)
            return;

        constexpr double edge = 0.95;
        constexpr double corner_offset = 0.05;

        const double span = edge - corner_offset;
        const double step = num > 1 ? (span * 2) / (num - 1) : 0.0;

        switch (align) {
            case BrainAlign::Top:
                step_p = {.x = step, .y = 0};
                first_p = {
                    .x = num > 1 ? -span : 0.0,
                    .y = edge
                };
                break;

            case BrainAlign::Right:
                step_p = {.x = 0, .y = -step};
                first_p = {
                    .x = edge,
                    .y = num > 1 ? span : 0.0
                };
                break;

            case BrainAlign::Bottom:
                step_p = {.x = -step, .y = 0};
                first_p = {
                    .x = num > 1 ? span : 0.0,
                    .y = -edge
                };
                break;

            case BrainAlign::Left:
                step_p = {.x = 0, .y = step};
                first_p = {
                    .x = -edge,
                    .y = num > 1 ? -span : 0.0
                };
                break;
        }
    }

public:
    explicit Agent(AgentBody *agent_body) : agent_body_(agent_body) {}

    void init(AgentConfig conf) {
        brain_.init(conf.genome, conf.num_of_common_neurons);

        // input
        for (auto [num, align] : conf.input_groups) {
            BrainPoint first_p{};
            BrainPoint step_p{};

            align_helper(align, num, first_p, step_p);

            for (auto i = 0; i < num; i++) {
                auto id = createInputNeuron(first_p, 0);
                input_neurons.push_back(id);

                first_p += step_p;
            }

        }

        // output
        for (auto [num, align] : conf.output_groups) {
            BrainPoint first_p{};
            BrainPoint step_p{};

            align_helper(align, num, first_p, step_p);

            for (auto i = 0; i < num; i++) {
                auto id = createOutputNeuron(first_p);
                output_neurons.push_back(id);

                first_p += step_p;
            }
        }
    }

    void handleSensors() {
        auto input = agent_body_->getInput();
        my_assert(input.size() == input_neurons.size());


        for (int i = 0; i < input.size(); i++) {
            auto id = input_neurons[i];
            auto intens = input[i];

            setInputNeuronIntensity(id, intens);
        }
    }

    void handleOutputs() {
        std::vector<bool> output; // intensity

        for (auto id : output_neurons) {
            auto neuron = brain_.getStorage().get<Neuron>(id);

            output.push_back(neuron->isSpiked());
        }

        agent_body_->handleOutput(output);
    }

    void tick() {
        // input neuron spike
        for (auto& [id, counter] : input_counters) {
            const int freq = input_freqs.at(id);
            if (freq == 0) continue;
            if (++counter >= freq) {
                brain_.getStorage().get<Neuron>(id)->manualSpike();
                counter = 0;
            }
        }

        brain_.tact();
    }

    Brain *getBrain() {
        return &brain_;
    }
};
