
#include <Brain.h>
#include <bits/this_thread_sleep.h>

int main() {
    Brain brain{};

    std::string neuron1_id{};
    std::string axon1_id{};
    std::string synaps_id{};
    std::string dendrite2_id{};
    std::string neuron2_id{};
    Neuron *neuron1{};


    std::unordered_map<std::string, std::string> res{};

    res = brain.createNeuron();
    neuron1_id = res.at("neuron");
    axon1_id = res.at("axon");

    res = brain.createNeuron();
    neuron2_id = res.at("neuron");
    dendrite2_id = res.at("dendrite");

    synaps_id = brain.createSynaps(axon1_id, dendrite2_id);

    neuron1 = brain.getCopyOfStorage().get<Neuron>(neuron1_id);

    // запускаем сеть (выстрел)
    neuron1->getSignal("10");

    for (int i = 0; i < 5; i++) {
        brain.tact();
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }

}
