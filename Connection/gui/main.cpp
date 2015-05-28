#include "mainwindow.h"
#include <QApplication>
#include "result.h"
INST_LOGGGER()
int main(int argc, char *argv[])
{
    if(argc < 3){
        std::cout << "use: ./mdp <ip> <port>" << std::endl;
        return 1;
    }
    QString ip(argv[1]);
    int port = atoi(argv[2]);
    Logger::addConsoleLogger();
    AsyncRequester<MainWindow::CallBack> requester(ip, port);
    QApplication a(argc, argv);
    MainWindow w(requester);
    w.show();
    LOG_INFO("Start");
    return a.exec();
}
