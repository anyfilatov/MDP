/* 
 * File:   SendGossipTask.h
 * Author: mikhail
 *
 * Created on May 2, 2015, 3:32 PM
 */

#ifndef SENDGOSSIPTASK_H
#define	SENDGOSSIPTASK_H

#include <QThread>
#include <QMutex>
#include "node.h"

class NetworkManager;

class SendGossipTask: public QThread {
    Q_OBJECT
public:
    SendGossipTask(NetworkManager& manager, QMutex& mutex);
    virtual ~SendGossipTask();
    
    void run();
    void sendNodeList(Node* me, QList<Node*>& nodeList);
    Node* selectPartner(QList<Node*>& nodeList);
private:
    NetworkManager& _manager;
    QMutex& _mutex;
};

#endif	/* SENDGOSSIPTASK_H */

