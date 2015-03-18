#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include  "prototypestorage.h"


class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

signals:

public slots:
    void newConnection();

private:
    QTcpServer *server;
    Storage<int, QString> *storage;

    void HandlerRequest(QTcpSocket *socket);
    void putHandler(QByteArray data);
    QByteArray getHandler(QByteArray data);
    QByteArray removeHandler(QByteArray data);
    QByteArray getAllHandler();
};

#endif // MYTCPSERVER_H
