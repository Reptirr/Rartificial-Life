
#include <AgentBody.h>
#include <MainWindow.h>
#include <QApplication>
#include <windows.h>
#include <World.h>

std::string fromColors(const std::vector<QColor>& colors) {
    std::string out;
    out.reserve(colors.size() * 24);   // примерно по 20-ти символов на блок

    for (const QColor& c : colors) {
        out += "\033[48;2;";
        out += std::to_string(c.red())   + ';';
        out += std::to_string(c.green()) + ';';
        out += std::to_string(c.blue())  + 'm';
        out += " ";                    // два пробела = цветной блок
    }
    out += "\033[0m";                   // один сброс в конце
    return out;
}

int main(int argc, char* argv[]) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    QApplication a{argc, argv};

    // MainWindow w;
    // w.show();

    World world;
    auto storage = world.getStorage();

    auto agentbody_id = world.addAgentBody({});
    auto agentbody = storage->getAgentBody(agentbody_id);

    // создаем препятствия
    world.addObstacle({5, 5});

    std::cout << fromColors(agentbody->getVision(0)) << std::endl;

    world.addObstacle({-5, 90});

    std::cout << fromColors(agentbody->getVision(0)) << std::endl;

    return a.exec();
}
