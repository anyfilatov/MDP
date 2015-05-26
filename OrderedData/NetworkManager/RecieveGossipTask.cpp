#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCore/qglobal.h>
#include <QDebug>

#include <iostream>
#include <QtCore/qlogging.h>
#include <QtCore/qlist.h>

#include "RecieveGossipTask.h"
#include "networkmanager.h"

RecieveGossipTask::RecieveGossipTask(NetworkManager& manager,
        QMutex& mutex, QObject *parent) : _manager(manager), _mutex(mutex) {
}

void RecieveGossipTask::start() {
    _socket = new QUdpSocket(this);
    QHostAddress address(_manager.getMyself()->getHost());
    qint16 port = _manager.getMyself()->getPort();
    _socket->bind(address, port);

    connect(_socket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

void RecieveGossipTask::readPendingDatagrams() {
    while (_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        _socket->readDatagram(datagram.data(), datagram.size(),
                &sender, &senderPort);

        processRemoteNodeList(datagram);
        //        qDebug() << "from: " << sender << " port: " << senderPort ;
    }
}

void RecieveGossipTask::processRemoteNodeList(const QByteArray& json) {
    int packet_length = 0;
    for (int i = 0; i < 4; i++) {
        int shift = (4 - 1 - i) * 8;
        packet_length += (json.at(i) & 0x000000FF) << shift;
    }

    if (packet_length <= NetworkManager::MAX_PACKET_LENGTH) {

        QJsonDocument document = QJsonDocument::fromBinaryData(json.mid(4));
        QJsonArray nodeList = document.array();
        //qDebug() << "Recieved: " << nodeList;
        QList<Node*> remoteNodes;
        //        Node *senderMember = NULL;

        for (int i = 0; i < nodeList.size(); ++i) {
            QJsonObject object = nodeList.at(i).toObject();
            if (object.length() == 3) {
                if ((object.value("port").toInt() == _manager.getMyself()->getPort()) 
                        && (object.value("host").toString() == _manager.getMyself()->getHost())) {
                    continue;
                }
                Node *member = new Node(
                        object.value("host").toString(),
                        object.value("port").toInt(),
                        object.value("heartbeat").toInt());
                remoteNodes.push_back(member);
            } else {
                std::cout << "The received member object does not contain 3 objects";
            }
        }

        _mutex.lock();
        mergeLists(_manager, remoteNodes);
        _mutex.unlock();
    }
}

void RecieveGossipTask::mergeLists(NetworkManager& manager, QList<Node*>& remoteNodeList) {
    for (Node* remoteNode : remoteNodeList) {
        Node* localNode = getLocalNodeByRemote(manager.getMemberList(), remoteNode);
        if (localNode != NULL) {
            if (localNode->getHeartbeat() < remoteNode->getHeartbeat()) {
                localNode->setHeartbeat(remoteNode->getHeartbeat());
            }
        } else {
            Node* deadLocalNode = getLocalNodeByRemote(manager.getDeadNodes(), remoteNode);
            if (deadLocalNode != NULL) {
                if (remoteNode->getHeartbeat() != deadLocalNode->getHeartbeat()) {
                    deadLocalNode->setHeartbeat(remoteNode->getHeartbeat());
                    manager.getMemberList().push_back(deadLocalNode);
                    manager.getDeadNodes().removeOne(deadLocalNode);
                }
            } else {
                Node * newLocalNode = new Node(remoteNode->getHost(),
                        remoteNode->getPort(),
                        remoteNode->getHeartbeat(),
                        manager.getSettings()->getCleanupInterval(),
                        manager);
                manager.getMemberList().push_back(newLocalNode);
                newLocalNode->startTimer();
            }
        }
    }
}

//QList<Node*> RecieveGossipTask::getRemoteNodesNotInLocal(QList<LocalNode>& localNodes,
//        const QList<Node>& remoteNodeList) {
//    QList<const Node*> inter;
//    for (int i = 0; i < remoteNodeList.size(); ++i) {
//        const Node& remoteNode = remoteNodeList[i];
//        bool contains = false;
//        for (int j = 0; j < localNodes.size(); ++j) {
//            LocalNode& localNode = localNodes[j];
//            if (localNode.getAddress() == remoteNode.getAddress())
//                contains = true;
//        }
//        if (!contains) {
//            inter.push_back(&remoteNode);
//        }
//    }
//}
//
//const Node* RecieveGossipTask::containsInRemoteList(LocalNode& node, const QList<Node>& remoteNodeList) {
//    const Node* result = NULL;
//    for (Node remoteNode : remoteNodeList) {
//        if (node.getAddress() == remoteNode.getAddress())
//            result = &(remoteNodeList[remoteNodeList.indexOf(remoteNode)]);
//    }
//    return result;
//}

Node* RecieveGossipTask::getLocalNodeByRemote(QList<Node*>& list, Node* node) {
    Node* result = NULL;
    for (Node* localNode : list) {
        if (localNode->getAddress() == node->getAddress())
            result = localNode;
    }
    return result;
}

RecieveGossipTask::~RecieveGossipTask() {

}

