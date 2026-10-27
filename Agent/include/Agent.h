#pragma once
#include <Brain.h>
#include <my_assert.h>
#include <Neuron.h>

/*
 * внешка сама создает ему входные/выходные нейроны. и сама их обрабатывает
 *
 * агент - прослойка между внешкой и мозгом, а также конвертер интенсивности -> частота спайков
 *
 */
class Agent {
    Brain brain_;

    // текущие частоты спайков на входные нейроны.
    // обозначает раз в сколько таков делать спайк
    std::unordered_map<std::string, int> input_freqs;

    // счетчик, показывающий через сколько делать спайк. per tact
    // при изменении частоты, он делает спайк а затем начинает с новой частотой
    std::unordered_map<std::string, int> input_counters;

    int convertIntensityToFreq(double intensity) {

        /*
         * если интенсивность = 1, то freq = 5
         * если интенсивность = 0.5, то freq = 10
         * если интенсивность = 0, то freq = 0
         */

        constexpr double MIN_PERIOD = 5.0;
        if (intensity <= 0.0) return 0;
        if (intensity > 1.0) intensity = 1.0;
        return static_cast<int>(std::round(MIN_PERIOD / intensity));
    }

public:
    void init(const Genome &genome, int numOfInitNeurons) {
        brain_.init(genome, numOfInitNeurons);
    }

    void tact() {
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

    /// @return neuron id
    std::string createInputNeuron(BrainPoint p, double init_intensity) {
        auto res = brain_.createInputNeuron(p);

        // init freq & counter
        int freq = convertIntensityToFreq(init_intensity);

        input_freqs.insert({res.at("neuron"), freq});
        input_counters.insert({res.at("neuron"), 1});

        return res.at("neuron");
    }

    /// @return neuron id
    std::string createOutputNeuron(BrainPoint p) {
        return brain_.createOutputNeuron(p).at("neuron");
    }

    void setInputNeuronIntensivity(const std::string &id, const double intensity) {
        my_assert(input_freqs.contains(id));
        my_assert(input_counters.contains(id));

        const int freq = convertIntensityToFreq(intensity);

        input_freqs.at(id) = freq;
    }

    Brain *getBrain() {
        return &brain_;
    }
};