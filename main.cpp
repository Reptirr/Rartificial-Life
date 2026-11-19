
#include <MainWindow.h>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[]) {
    // сглаживание
    QSurfaceFormat fmt;
    fmt.setSamples(16);                       
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a{argc, argv};

    MainWindow w;
    w.show();


    return a.exec();
}
