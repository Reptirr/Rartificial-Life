
#include <Brain.h>
#include <BrainWidget.h>
#include <QApplication>
#include <QTimer>
#include <bits/this_thread_sleep.h>

int main(int argc, char** argv) {
    Brain brain{4};
    auto &storage = brain.getStorage();


    // создаем рандомные синапсы
    for (int i = 0; i < 6; i++) {
        std::string axon, dendrite;

        do {
            axon = storage.getRandomAxon();
            dendrite = storage.getRandomDendrite();
        } while (storage.get<Axon>(axon)->getNeuron() == storage.get<Dendrite>(dendrite)->getNeuron());

        brain.createSynaps(axon, dendrite);
    }

    QApplication a{argc, argv};

    auto *w = new BrainWidget(storage);
    w->setFixedSize(1000, 1000);
    w->show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        brain.tact();
        w->update();

        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    });
    timer.start(0);

    return a.exec();
}
