#pragma once

#include <AgentConfig.h>
#include <Brain.h>
#include <Uuid.h>

class AgentBody;

class Agent {
    AgentConfig conf_;

    Brain brain_;
    AgentBody *agent_body_{};

    // --------------
    // INPUT
    // --------------

    // текущие частоты спайков на входные нейроны.
    // обозначает раз в сколько таков делать спайк
    std::unordered_map<Uuid, int, UuidHash> input_freqs;

    // счетчик, показывающий через сколько делать спайк. per tact
    // при изменении частоты, он делает спайк а затем начинает с новой частотой
    std::unordered_map<Uuid, int, UuidHash> input_counters;

    std::vector<Uuid> input_neurons;
    std::vector<Uuid> output_neurons;



    int convertIntensityToFreq(double intensity);
    void setInputNeuronIntensity(const Uuid &id, const double intensity);
    bool getNeuronSpiked(const Uuid &id);

    Uuid createInputNeuron(BrainPoint p, double init_intensity);
    Uuid createOutputNeuron(BrainPoint p);

    // utils
    static void align_helper(BrainAlign align, int num, BrainPoint &first_p, BrainPoint &step_p);

public:
    explicit Agent(AgentBody *agent_body) : agent_body_(agent_body) {}

    void init(AgentConfig conf);

    void handleSensors();
    void handleOutputs();

    void tick();

    Brain *getBrain();
    AgentConfig getConfig() {
        return conf_;
    }
};
