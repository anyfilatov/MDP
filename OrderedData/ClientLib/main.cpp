#include <QCoreApplication>
#include <QDebug>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    client.put<QString, QString>("test", "test", "TEST");
    qDebug() << client.get<QString, QString>("test");
    client.remove("TEST");
    qDebug() << client.get<QString, QString>("test");
    return a.exec();
}


