#ifndef ABSTRACTCLIENT_H
#define ABSTRACTCLIENT_H

#include <QTcpSocket>
#include <QString>
#include <QJsonObject>

class AbstractClient
{
public:
    AbstractClient();
    ~AbstractClient();
    virtual bool connectToHost(const QString& host, quint16 port);
    virtual void write(const QJsonObject& object);
    virtual QJsonObject read();
    virtual void disconnectFromHost();
protected:
    QTcpSocket _socket;
    static const int MAX_WAIT_TIME_MLS = 30000;
};

#endif // ABSTRACTCLIENT_H
