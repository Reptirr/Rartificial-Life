
#include <Brain.h>
#include <BrainWidget.h>
#include <QApplication>

int main(int argc, char** argv) {
    Brain brain{25};
    auto &storage = brain.getStorage();


    // создаем рандомные синапсы
    for (int i = 0; i < 50; i++) {
        std::string axon, dendrite;

        do {
            axon = storage.getRandomAxon();
            dendrite = storage.getRandomDendrite();
        } while (storage.get<Axon>(axon)->getNeuron() == storage.get<Dendrite>(dendrite)->getNeuron());

        brain.createSynaps(axon, dendrite);
    }

/*
    // запускаем сеть (выстрел)
    storage.get<Neuron>(storage.getRandomNeuron())->addSignal("13");


    for (;;) {
        brain.tact();
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
*/

    QApplication a{argc, argv};

    auto *w = new BrainWidget(storage);
    w->setFixedSize(1000, 1000);
    w->show();

    return a.exec();
}
