#include "tcpserver.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
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
        qDebug() << "traverseHandler";
        data.append(pack.right(1024));
        QByteArray test = this->traverseHandler(data);
        socket->write(test);
        socket->flush();
    }

    if (pack[0] == 53 && pack[1] == 48)
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
    //QString str(data);
    QList<QByteArray> list = data.split('#');
    qDebug() <<"\tData key: " << list.first();
    qDebug() <<"\tData value: " << list.last();

}

QByteArray TcpServer::getHandler(QByteArray key)
{
    qDebug() <<"\tData key: " <<key;
    QByteArray data;
    data.append("testGet");
    return data;

}

QByteArray TcpServer::removeHandler(QByteArray key)
{
    qDebug() <<"\tData key: " <<key;
    QByteArray data;
    data.append("testRemove");
    return data;
}

QByteArray TcpServer::traverseHandler(QByteArray data)
{
    QList<QByteArray> list = data.split('#');
    qDebug() <<"\tData intStart: " <<list.first().toInt();
    qDebug() <<"\tData intEnd: " <<list.last().toInt();
    QByteArray test;
    test.append("testTraverse");
    return test;

}

QByteArray TcpServer::getAllHandler()
{
    QByteArray data;
    data.append("testGetAll");
    return data;
}
