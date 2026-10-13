
#include <Brain.h>
#include <bits/this_thread_sleep.h>

int main() {
    Brain brain{5};
    auto &storage = brain.getStorage();

    // создаем рандомные синапсы
    for (int i = 0; i < 100; i++) {
        std::string axon, dendrite;

        do {
            axon = storage.getRandomAxon();
            dendrite = storage.getRandomDendrite();
        } while (storage.get<Axon>(axon)->getNeuron() == storage.get<Dendrite>(dendrite)->getNeuron());

        brain.createSynaps(axon, dendrite);
    }

    // запускаем сеть (выстрел)
    storage.get<Neuron>(storage.getRandomNeuron())->addSignal("13");


    for (;;) {
        brain.tact();
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }

}
