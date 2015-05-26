#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class Server : public QTcpServer
{
    Q_OBJECT

public slots:
    void read();
public:
    Server();
    void run();

private:
    QTcpSocket* socket;
    QDataStream* stream;
    quint32 packetSize = 0;
};

#endif // SERVER_H
