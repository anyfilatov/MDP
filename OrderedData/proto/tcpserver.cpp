#include "tcpserver.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    server = new QTcpServer(this);
    storage = new PrototypeStorage<int, QString>();
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
        this->getHandler(data);
        socket->write("get");
        socket->flush();
    }
    if (pack[0] == 51 && pack[1] == 48)
    {
        qDebug() << "removeHandler";
        data.append(pack.right(1024));
        this->removeHandler(data);
        socket->write("remove");
        socket->flush();
    }
    if (pack[0] == 52 && pack[1] == 48)
    {
        qDebug() << "traverseHandler";
        data.append(pack.right(1024));
        this->traverseHandler(data);
        socket->write("traverse");
        socket->flush();
    }

    if (pack[0] == 53 && pack[1] == 48)
    {
        qDebug() << "getAllHandler";
        data.append(pack.right(1024));
        this->getAllHandler(data);
        socket->write("getAll");
        socket->flush();
    }

    socket->waitForBytesWritten(3000);

    socket->close();
}

void TcpServer::putHandler(QByteArray data)
{
    //QString str(data);
    QList<QByteArray> list = data.split('#');
    qDebug() <<"\tData key: " << list.first();
    qDebug() <<"\tData value: " << list.last();
    storage->put(list.first().toInt(), list.last());
}

void TcpServer::getHandler(QByteArray data)
{
    qDebug() <<"\tData key: " <<data;
    storage->get(data.toInt());
}

void TcpServer::removeHandler(QByteArray data)
{
    qDebug() <<"\tData key: " <<data;
    qDebug() <<"\tData value: " << storage->remove(data.toInt());
}

void TcpServer::traverseHandler(QByteArray data)
{
    QList<QByteArray> list = data.split('#');
    qDebug() <<"\tData intStart: " <<list.first().toInt();
    qDebug() <<"\tData intEnd: " <<list.last().toInt();
    storage->traverse(list.first().toInt(), list.last().toInt());
}

void TcpServer::getAllHandler()
{
    storage->getAll();
}
