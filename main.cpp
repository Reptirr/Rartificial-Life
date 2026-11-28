
#include <AppStyle.h>
#include <MainWindow.h>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[]) {
    // сглаживание
    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication a{argc, argv};

    // свой стиль
    a.setStyle(new AppStyle);

    MainWindow w;
    w.show();


    return a.exec();
}
