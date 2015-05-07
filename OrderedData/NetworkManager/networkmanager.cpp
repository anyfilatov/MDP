#include "networkmanager.h"
#include <QDebug>
#include <QtCore/qglobal.h>

NetworkManager::NetworkManager(NetworkSettings& settings) {
    qDebug() << "Starting manager...";
    _settings = new NetworkSettings(settings);
    _me = new Node(_settings->getHost(), _settings->getPort(), 0, _settings->getCleanupInterval(), *this);

    QList<QPair<QString, qint16> > members = _settings->getGossipNodes();
    for (QPair<QString, qint16> startupMember : members) {
        Node *member = new Node(startupMember.first,
                startupMember.second, 0, _settings->getCleanupInterval(), *this);
        _memberList.push_back(member);
    }
    
    for (Node* member : _memberList) {
        member->startTimer();
    }

    _sendGossipTask = new SendGossipTask(*this, _mutex);
    _sendGossipTask->start();

    _recieveGossipTask = new RecieveGossipTask(*this, _mutex);
    _recieveGossipTask->start();
    qDebug() << "Started...";
    qDebug() << QThread::currentThreadId();
}

NetworkManager::~NetworkManager() {

}

QList<Node*>& NetworkManager::getMemberList() {
    return _memberList;
}

QList<Node *> NetworkManager::getAllMembers()
{
    QList<Node*> allNodes;
    allNodes.push_back(_me);
    for (Node* node: _memberList) {
        allNodes.append(node);
    }

    return allNodes;
}

QList<Node*>& NetworkManager::getDeadNodes() {
    return _deadList;
}

Node * NetworkManager::getMyself() {
    return _me;
}

const NetworkSettings* NetworkManager::getSettings() const {
    return _settings;
}

void NetworkManager::handleDeadNode(QString address) {
    qDebug() << "DELETE DEAD NODE: " << address;
    for (Node* node: _memberList) {
        if (node->getAddress() == address) {
            _deadList.push_back(node);
            _memberList.removeOne(node);
            break;
        }
    }
}


