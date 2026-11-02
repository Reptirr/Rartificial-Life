#pragma once

#include <AgentBody.h>
#include <Model.h>

class Agent {
    Brain brain_;
    AgentBody *agent_body_;

    // текущие частоты спайков на входные нейроны.
    // обозначает раз в сколько таков делать спайк
    std::unordered_map<std::string, int> input_freqs;

    // счетчик, показывающий через сколько делать спайк. per tact
    // при изменении частоты, он делает спайк а затем начинает с новой частотой
    std::unordered_map<std::string, int> input_counters;

    // входные нейроны зрения
    std::vector<std::string> input_vision_neurons;

    // выходные нейроны движения
    int move_directions{};
    std::vector<std::string> output_move_neurons;

    // сглаженная частота спайков выходных нейронов (спайков за такт)
    std::vector<float> output_smooth;

    static constexpr float SMOOTH_K  = 0.1f;   // меньше = плавнее, но медленнее реакция
    static constexpr float MAX_ACCEL = 10.0f;  // подбери под свою физику

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
    void setInputNeuronIntensivity(const std::string &id, const double intensity) {
        my_assert(input_freqs.contains(id));
        my_assert(input_counters.contains(id));

        const int freq = convertIntensityToFreq(intensity);

        input_freqs.at(id) = freq;
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

public:
    explicit Agent(AgentBody *agent_body) : agent_body_(agent_body) {}

    void init(AgentDef conf, int numOfInitNeurons) {
        brain_.init(conf.genome, numOfInitNeurons);

        // создаем входные зрительные нейроны
        for (auto p : conf.vision_neurons) { // 3 цвета
            auto id = createInputNeuron(p, 0);
            input_vision_neurons.push_back(id);
        }

        // создаем выходные нейроны движения
        for (auto p : conf.move_neurons) {
            auto id = createOutputNeuron(p);
            output_move_neurons.push_back(id);
        }
        move_directions = conf.directions;

        output_smooth.assign(output_move_neurons.size(), 0.0f);
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
            int canal{};
            switch (i%3) {
                case 0: canal = color.red(); break;
                case 1: canal = color.green(); break;
                case 2: canal = color.blue(); break;
            }

            // конвертируем в интенсивность
            const double intens = canal / 255.0;

            // говорим нейрону его цвет
            brain_.getStorage().get<Neuron>(id)->setColor(color);

            // применяем на нейроне
            setInputNeuronIntensivity(id, intens);

            i++;
        }
    }

    // вызывать ровно один раз за такт, после tick()
    void handleOutputs() {
        my_assert(static_cast<int>(output_move_neurons.size()) == move_directions);

        b2Vec2 vec{0.0f, 0.0f};

        for (int i = 0; i < move_directions; i++) {
            auto *neuron = brain_.getStorage().get<Neuron>(output_move_neurons[i]);

            // спайки с прошлого вызова (счетчик сбрасывается)
            const int spikes = neuron->consumeSpikes();
            const float max_freq = 1.0f / std::max(1, neuron->getRefractorTicks());

            // «мышца»: скользящее среднее по спайкам
            output_smooth[i] += (static_cast<float>(spikes) - output_smooth[i]) * SMOOTH_K;

            // нормируем в [0, 1]
            const float out = std::min(output_smooth[i] / max_freq, 1.0f);

            // направление нейрона i на окружности
            const float angle = 2.0f * B2_PI * i / move_directions;
            vec.x += std::cos(angle) * out;
            vec.y += std::sin(angle) * out;
        }

        // противоположные направления гасят друг друга,
        // но суммарная длина может превысить 1
        const float len = b2Length(vec);
        if (len > 1.0f) vec = b2MulSV(1.0f / len, vec);

        agent_body_->applyAcceleration(b2MulSV(MAX_ACCEL, vec));
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
