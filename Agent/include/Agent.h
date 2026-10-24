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

    int convertIntensityToFreq(const double intensity) {
        const int res = intensity * (1 / 5); // refractor = 5
        return res;    }

public:
    explicit Agent() {
        brain_.init(200);
    }

    void tact() {
        // input neuron spike
        for (auto& [id, counter] : input_counters) {
            counter--;

            if (counter <= 0) {
                brain_.getStorage().get<Neuron>(id)->manualSpike();
                counter = input_freqs.at(id);
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
        input_counters.insert({res.at("neuron"), freq});

        return res.at("neuron");
    }

    /// @return neuron id
    std::string createOutputNeuron(BrainPoint p) {
        return brain_.createOutputNeuron(p).at("neuron");
    }

    void setInputNeuronIntensivity(std::string id, double intensity) {
        my_assert(input_freqs.contains(id));
        my_assert(input_counters.contains(id));

        int freq = convertIntensityToFreq(intensity);

        input_freqs.at(id) = freq;
    }

    Brain *getBrain() {
        return &brain_;
    }
};