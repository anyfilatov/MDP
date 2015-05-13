#ifndef CONNECT_H
#define CONNECT_H

#include <QRunnable>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QObject>
#include <QTimer>
#include "Cache/cache.h"
#include "Router/hashring.h"

class Connect : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Connect(HashRing *ring, Cache<QString, QString> *_rbtree, QObject *parent = 0);
    ~Connect();

protected:
    void run();

public:
    qintptr socketDescriptor;

private:
    Cache<QString, QString> *_rbtree;
    QTcpSocket *_socket;
    HashRing *_ring;

    QJsonObject handleRequest(QJsonObject json);
    QJsonObject handlePut(QJsonObject json);
    QJsonObject handleReplace(QJsonObject json);
    QJsonObject handleGet(QJsonObject json);
    QJsonObject handleGetBucket(QJsonObject json);
    QJsonObject handleRemove(QJsonObject json);
    QJsonObject handleRemoveBucket(QJsonObject json);
    QJsonObject handleRingCheck(QJsonObject json);
    QJsonObject handleRingJoin(QJsonObject json);

    QByteArray serialize(QJsonObject json);
    QJsonObject deserialize(QByteArray data);
};

#endif
