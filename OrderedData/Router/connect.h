#ifndef CONNECT_H
#define CONNECT_H

#include <QRunnable>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "../Cache/icache.h"

class Connect : public QRunnable
{

public:
    Connect(iCache<QString, QString> *rbtree);

protected:
    void run();

public:
    qintptr socketDescriptor;

private:
    iCache<QString, QString> *rbtree;

    QJsonObject deserialization(QByteArray data);
    QByteArray serialization(QJsonObject data);
    QJsonObject handleRequest(QJsonObject json);
    QJsonObject handlePut(QJsonObject json);
    QJsonObject handleGet(QJsonObject json);
    QJsonObject handleDelete(QJsonObject json);
    QJsonObject handleRingCheck(QJsonObject json);
    QJsonObject handleOuterJoin(QJsonObject json);


    QByteArray createMessage(QJsonObject json);
    QJsonObject parseMessage(QByteArray data);
};

#endif
