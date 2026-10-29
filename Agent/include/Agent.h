#pragma once
#include <AgentBody.h>
#include <Brain.h>
#include <my_assert.h>
#include <Neuron.h>

struct AgentConfig {
    Genome genome;

    int eyes{};
    int pixels_in_eye{};
    BrainPoint first_input_neuron_pos;
    BrainPoint space_between_input_neurons_;
};

/*
 * внешка сама создает ему входные/выходные нейроны. и сама их обрабатывает
 *
 * агент - прослойка между внешкой и мозгом, а также конвертер интенсивности -> частота спайков
 *
 */
class Agent {
    Brain brain_;
    AgentBody *agent_body_;

    // текущие частоты спайков на входные нейроны.
    // обозначает раз в сколько таков делать спайк
    std::unordered_map<std::string, int> input_freqs;

    // счетчик, показывающий через сколько делать спайк. per tact
    // при изменении частоты, он делает спайк а затем начинает с новой частотой
    std::unordered_map<std::string, int> input_counters;

    AgentConfig conf_{};

    // входные нейроны зрения
    std::unordered_set<std::string> input_vision_neurons;

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

public:
    explicit Agent(AgentBody *agent_body) : agent_body_(agent_body) {}

    void init(AgentConfig conf, int numOfInitNeurons) {
        conf_ = conf;
        brain_.init(conf.genome, numOfInitNeurons);

        BrainPoint pos = conf.first_input_neuron_pos;

        // создаем входные зрительные нейроны
        for (auto i = 0; i < conf.eyes * conf.pixels_in_eye * 3; i++) { // 3 цвета
            auto id = createInputNeuron(pos, 0);
            input_vision_neurons.insert(id);

            pos += conf.space_between_input_neurons_;
        }
    }

    void resolveSensors() {
        // зрение
        auto colors = agent_body_->getVision(0);
        my_assert(colors.size()*3 == input_vision_neurons.size());

        int i = 0;
        for (auto id : input_vision_neurons) {
            // получаем цвет
            auto color = colors.at(i / 3); // дробная часть отбрасывается

            // получаем значение канала
            double canal{};
            switch (i%3) {
                case 0: canal = color.red(); break;
                case 1: canal = color.green(); break;
                case 2: canal = color.blue(); break;
            }

            // конвертируем в интенсивность
            const double intens = canal / 255;

            // говорим нейрону его цвет
            brain_.getStorage().get<Neuron>(id)->setColor(color);

            // применяем на нейроне
            setInputNeuronIntensivity(id, intens);

            i++;
        }
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