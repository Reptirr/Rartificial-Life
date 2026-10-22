
#include <Brain.h>
#include <BrainView.h>
#include <QApplication>
#include <QTimer>
#include <QWidget>
#include <QHBoxLayout>

int main(int argc, char** argv) {
    Brain brain{};
    auto &storage = brain.getStorage();

    QApplication a{argc, argv};

    auto *w = new QWidget();
    w->setFixedSize(1000, 1000);
    w->show();

    auto *brainView = new BrainView(w, storage);
    brainView->setFixedSize(1000, 1000);
    auto *layout = new QHBoxLayout(w);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(brainView);


    const std::function onNewNeuron = [&](const std::string &neuron) {
        brainView->onNewNeuron(neuron);
    };
    const std::function onNewSynaps = [&](const std::string &synaps) {
        brainView->onNewSynaps(synaps);
    };
    const auto onUpdateAxon = [&](const std::string &axon) {
        brainView->onUpdateAxon(axon);
    };
    const auto onUpdateDendrite = [&](const std::string &dendrite) {
        brainView->onUpdateDendrite(dendrite);
    };

    brain.setNewNeuronCallback(onNewNeuron);
    brain.setNewSynapsCallback(onNewSynaps);
    brain.setUpdateAxonCallback(onUpdateAxon);
    brain.setUpdateDendriteCallback(onUpdateDendrite);

    // start
    brain.init(200);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        brain.tact();
    });
    timer.start(0);

    return a.exec();
}
