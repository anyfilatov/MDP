#include "SendGossipTask.h"

#include <QThread>
#include <QTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork/QUdpSocket>
#include <QDebug>

#include <iostream>
#include <QtCore/qglobal.h>

#include "networkmanager.h"

SendGossipTask::SendGossipTask(NetworkManager& manager,
        QMutex& mutex) : _manager(manager), _mutex(mutex) {
    qDebug() << "Send task initialized";
}

void SendGossipTask::run() {
    //    qDebug() << "Running sending task";
    while (true) {
        //        qDebug() << "Trying to send list";
        QThread::currentThread()
                ->msleep(_manager.getSettings()->getGossipInterval());
        sendNodeList(_manager.getMyself(), _manager.getMemberList());
    }
}

void SendGossipTask::sendNodeList(Node* me,
        QList<Node*>& nodeList) {
    me->setHeartbeat(me->getHeartbeat() + 1);
    
    _mutex.lock();
    Node* node = selectPartner(nodeList);
    if (node != NULL) {
        QHostAddress dst(node->getHost());
        QJsonArray json;
        json.push_back(QJsonValue(me->toJsonObject()));

        for (Node* n: nodeList) {
            json.push_back(QJsonValue(n->toJsonObject()));
        }
        //qDebug() << "Sending list: " << json;
        QByteArray json_bytes = (new QJsonDocument(json))->toBinaryData();
        int packet_length = json_bytes.size();
        if (packet_length <= NetworkManager::MAX_PACKET_LENGTH) {
            char length_bytes[4];
            length_bytes[0] = (char) (packet_length >> 24);
            length_bytes[1] = (char) ((packet_length << 8) >> 24);
            length_bytes[2] = (char) ((packet_length << 16) >> 24);
            length_bytes[3] = (char) ((packet_length << 24) >> 24);

            QByteArray packet;
            packet.append(length_bytes, 4);
            packet.append(json_bytes);

            QUdpSocket socket;
            socket.writeDatagram(packet, dst, node->getPort());
            //            qDebug() << "Sent from: " << me.getAddress() << "to: " << node->getAddress();
        }
    } else {
        qDebug() << "I'm alone in this world\n";
    }
    _mutex.unlock();
}

Node* SendGossipTask::selectPartner(QList<Node*>& nodeList) {
    Node* partner = NULL;
    if (!nodeList.empty()) {
        QTime midnight(0, 0, 0);
        qsrand(midnight.secsTo(QTime::currentTime()));
        partner = nodeList[qrand() % nodeList.size()];
    } else {
        std::cout << "I am alone";
    }
    return partner;
}

SendGossipTask::~SendGossipTask() {
}

