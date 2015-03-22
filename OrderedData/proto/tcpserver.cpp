#include "tcpserver.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    storage = new PrototypeStorage<int, QString>;
    storage->put(123,"test1");
    storage->put(123,"test2");
    storage->put(123,"test3");
    storage->put(987,"test9");
    storage->put(987,"test8");
    server = new QTcpServer(this);
    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any, 10000))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void TcpServer::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();
    this->HandlerRequest(socket);
}

void TcpServer::HandlerRequest(QTcpSocket *socket)
{
    QByteArray pack;
    QByteArray data;

    socket->waitForReadyRead();
    while (socket->bytesAvailable())
    {
        pack.append(socket->readAll());
    }

    if (pack[0] == 49 && pack[1] == 48)
    {
        qDebug() << "putHandler";
        data.append(pack.right(1024));
        this->putHandler(data);
        socket->write("put");
        socket->flush();
    }
    if (pack[0] == 50 && pack[1] == 48)
    {
        qDebug() << "getHandler";
        data.append(pack.right(1024));
        QByteArray test = this->getHandler(data);
        socket->write(test);
        socket->flush();
    }
    if (pack[0] == 51 && pack[1] == 48)
    {
        qDebug() << "removeHandler";
        data.append(pack.right(1024));
        QByteArray test = this->removeHandler(data);
        socket->write(test);
        socket->flush();
    }
    if (pack[0] == 52 && pack[1] == 48)
    {
        qDebug() << "getAllHandler";
        data.append(pack.right(1024));
        QByteArray test = this->getAllHandler();
        socket->write(test);
        socket->flush();
    }

    socket->waitForBytesWritten(3000);

    socket->close();
}

void TcpServer::putHandler(QByteArray data)
{
    QList<QByteArray> list = data.split('#');
    qDebug() <<"\tData key: " << list.first();
    qDebug() <<"\tData value: " << list.last();
    storage->put(list.first().toInt(), list.last());
}

QByteArray TcpServer::getHandler(QByteArray key)
{
    qDebug() <<"\tData key: " << key;
    QList<QString> value = storage->get(key.toInt());

    QByteArray data;
    foreach (QString s, value) {
        data.append(s);
        data.append('#');
    }

    return data;

}

QByteArray TcpServer::removeHandler(QByteArray key)
{
    qDebug() <<"\tData key: " << key;
    int code = storage->remove(key.toInt());
    QByteArray data;
    data.append(QString::number(code));
    return data;
}

QByteArray TcpServer::getAllHandler()
{
    QList<QPair<int, QList<QString> > > list = storage->getAll();
    QPair<int, QList<QString> > pair;
    QByteArray data;
    foreach (pair, list) {
        int key = pair.first;
        data.append(QString::number(key));
        data.append(":");
        foreach (QString str, pair.second) {
            data.append(str);
            data.append('#');
        }
        data.append('|');
    }
    return data;
}
