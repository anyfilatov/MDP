#ifndef CONNECT_H
#define CONNECT_H

#include <QRunnable>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include "../Cache/icache.h"

class Connect : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Connect(iCache<QString, QString> *rbtree, QObject *parent = 0);
    ~Connect();

protected:
    void run();

public:
    qintptr socketDescriptor;

private:
    iCache<QString, QString> *rbtree;
    QTcpSocket *socket;

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
