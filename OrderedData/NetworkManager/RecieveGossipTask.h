#ifndef RECIEVEGOSSIPTASK_H
#define	RECIEVEGOSSIPTASK_H

#include <QObject>
#include <QThread>
#include <QtNetwork/QUdpSocket>
#include <QMutex>

#include "node.h"

class NetworkManager;

class RecieveGossipTask: public QObject {
    Q_OBJECT
public:
    explicit RecieveGossipTask(NetworkManager& manager, QMutex& mutex, QObject *parent=0);
    virtual ~RecieveGossipTask();
    void start();
    void processRemoteNodeList(const QByteArray& json);
    void mergeLists(NetworkManager& manager, QList<Node*>& remoteNodeList);
public slots:
    void readPendingDatagrams();
private:
    Node* getLocalNodeByRemote(QList<Node*>& list, Node* node);
    QUdpSocket* _socket;
    NetworkManager& _manager;
    QMutex& _mutex;

};

#endif	/* RECIEVEGOSSIPTASK_H */

