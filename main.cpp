
#include <Brain.h>
#include <BrainWidget.h>
#include <QApplication>
#include <QTimer>
#include <bits/this_thread_sleep.h>

int main(int argc, char** argv) {
    Brain brain{2000};
    auto &storage = brain.getStorage();

    QApplication a{argc, argv};

    auto *w = new BrainWidget(storage);
    w->setFixedSize(1000, 1000);
    w->show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        w->update();
        brain.tact();
    });
    timer.start(0);

    return a.exec();
}
