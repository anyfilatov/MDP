#include <QCoreApplication>
#include <QDebug>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    qDebug() << "Put <testKey, testValue>: " << client.put<QString, QString>("testKey", "testValue", "TEST");
    qDebug() << "Get <testKey>: " << client.get<QString, QString>("testKey");
    qDebug() << "Del <testKey>: " << client.remove("TEST");
    qDebug() << "Get <testKey>: " << client.get<QString, QString>("testKey");
    return a.exec();
}


