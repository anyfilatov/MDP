#ifndef CONNECT_H
#define CONNECT_H

#include <QRunnable>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QObject>
#include <QMutex>
#include "Cache/cache.h"
#include "Router/hashring.h"

class Connection : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Connection(qintptr handle,
               HashRing *ring,
               Cache<QString, QString> *_rbtree,
               QMutex* mutex,
               QString myAddress,
               QObject *parent = 0);
    ~Connection();

protected:
    void run();

private:
    Cache<QString, QString> *_rbtree;
    QTcpSocket *_socket;
    HashRing *_ring;
    qintptr _socketDescriptor;
    QMutex *_mutex;
    QString _me;
    int _count = 1;

    QJsonObject handleRequest(const QJsonObject& json);
    void handleBatchRequest(const QJsonObject& json);
    QJsonObject handleGet(const QJsonObject& json);
    QJsonObject handleRingCheck();
    QJsonObject handleRingJoin(const QJsonObject& json);

    void write(const QJsonObject& object);
    QJsonObject read();

    bool isConnected();
};

#endif
