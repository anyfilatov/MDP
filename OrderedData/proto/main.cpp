#include <QCoreApplication>
#include "tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    // create MyTcpServer
    // MyTcpServer constructor will create QTcpServer

    TcpServer server;

    return a.exec();
}