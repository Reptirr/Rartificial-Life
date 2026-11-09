#include <Agent.h>
#include <AgentBody.h>
#include <Neuron.h>


int Agent::convertIntensityToFreq(double intensity) {
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

void Agent::setInputNeuronIntensity(const std::string &id, const double intensity) {
    my_assert(input_freqs.contains(id));
    my_assert(input_counters.contains(id));

    const int freq = convertIntensityToFreq(intensity);

    input_freqs.at(id) = freq;

    auto *neuron = brain_.getStorage().get<Neuron>(id);
    neuron->setIntensity(intensity);
}

bool Agent::getNeuronSpiked(const std::string &id) {
    auto *neuron = brain_.getStorage().get<Neuron>(id);
    auto isSpiked = neuron->isSpiked();

    return isSpiked;
}

std::string Agent::createInputNeuron(BrainPoint p, double init_intensity) {
    auto res = brain_.createInputNeuron(p);

    // init freq & counter
    int freq = convertIntensityToFreq(init_intensity);

    input_freqs.insert({res.at("neuron"), freq});
    input_counters.insert({res.at("neuron"), 1});

    return res.at("neuron");
}

std::string Agent::createOutputNeuron(BrainPoint p) {
    return brain_.createOutputNeuron(p).at("neuron");
}

void Agent::align_helper(BrainAlign align, int num, BrainPoint &first_p, BrainPoint &step_p) {
    if (num <= 0)
        return;

    constexpr double edge = 0.95;
    constexpr double corner_offset = 0.05;
    constexpr double span = edge - corner_offset;
    constexpr double mid_gap = corner_offset;   // зазор в середине стороны между половинками

    enum class Side { Top, Right, Bottom, Left };
    Side side = Side::Top;
    double t_from = -span;   // отрезок вдоль стороны, по часовой
    double t_to   =  span;

    switch (align) {
        // целые стороны
        case BrainAlign::Top:    side = Side::Top;    break;
        case BrainAlign::Right:  side = Side::Right;  break;
        case BrainAlign::Bottom: side = Side::Bottom; break;
        case BrainAlign::Left:   side = Side::Left;   break;

        // первая по ходу часовой стрелки половина стороны
        case BrainAlign::Top_Left:     side = Side::Top;    t_to = -mid_gap; break;
        case BrainAlign::Right_Top:    side = Side::Right;  t_to = -mid_gap; break;
        case BrainAlign::Bottom_Right: side = Side::Bottom; t_to = -mid_gap; break;
        case BrainAlign::Left_Bottom:  side = Side::Left;   t_to = -mid_gap; break;

        // вторая половина
        case BrainAlign::Top_Right:    side = Side::Top;    t_from = mid_gap; break;
        case BrainAlign::Right_Bottom: side = Side::Right;  t_from = mid_gap; break;
        case BrainAlign::Bottom_Left:  side = Side::Bottom; t_from = mid_gap; break;
        case BrainAlign::Left_Top:     side = Side::Left;   t_from = mid_gap; break;
    }

    const double step = num > 1 ? (t_to - t_from) / (num - 1) : 0.0;
    const double t0   = num > 1 ? t_from : (t_from + t_to) / 2.0;   // один нейрон по центру отрезка

    switch (side) {
        case Side::Top:
            first_p = {.x =  t0,   .y =  edge};
            step_p  = {.x =  step, .y =  0};
            break;
        case Side::Right:
            first_p = {.x =  edge, .y = -t0};
            step_p  = {.x =  0,    .y = -step};
            break;
        case Side::Bottom:
            first_p = {.x = -t0,   .y = -edge};
            step_p  = {.x = -step, .y =  0};
            break;
        case Side::Left:
            first_p = {.x = -edge, .y =  t0};
            step_p  = {.x =  0,    .y =  step};
            break;
    }
}

void Agent::init(AgentConfig conf) {
    conf_ = conf;
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

void Agent::handleSensors() {
    auto input = agent_body_->getInput();
    my_assert(input.size() == input_neurons.size());


    for (int i = 0; i < input.size(); i++) {
        auto id = input_neurons[i];
        auto intens = input[i];

        setInputNeuronIntensity(id, intens);
    }
}

void Agent::handleOutputs() {
    std::vector<bool> output; // intensity

    for (auto id : output_neurons) {
        auto neuron = brain_.getStorage().get<Neuron>(id);

        output.push_back(neuron->isSpiked());
    }

    agent_body_->handleOutput(output);
}

void Agent::tick() {
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

Brain * Agent::getBrain() {
    return &brain_;
}
