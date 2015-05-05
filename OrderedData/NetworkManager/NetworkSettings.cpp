/* 
 * File:   NetworkSettings.cpp
 * Author: mikhail
 * 
 * Created on May 1, 2015, 5:32 PM
 */

#include "NetworkSettings.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>

NetworkSettings::NetworkSettings(QString host, qint32 port, 
        qint32 gossipInterval, 
        qint32 cleanupInterval) {
    _host = host;
    _port = port;
    _gossipInterval = gossipInterval;
    _cleanupInterval = cleanupInterval;
}

NetworkSettings::NetworkSettings(QFile& settings) {
    QJsonParseError *parseError = NULL;
    QJsonDocument document = QJsonDocument::fromJson(settings.readAll(), parseError);
    if (parseError == NULL) {
        QJsonObject json = document.object();
        _host = json.value("host").toString();
        _port = json.value("port").toInt();
        _gossipInterval = json.value("gossip_interval").toInt(DEFAULT_GOSSIP_INTERVAL);
        _cleanupInterval= json.value("cleanup_interval").toInt(DEFAULT_CLEANUP_INTERVAL);
        QJsonArray members = json.value("members").toArray();
        for (int i = 0; i < members.size(); i++) {
            QJsonObject member = members.at(i).toObject();
            if ((member.value("port").toInt() == _port) && (member.value("host").toString() == _host)) {
                continue;
            }
               
            QPair<QString, qint16> node = qMakePair(member.value("host").toString(),
                    member.value("port").toInt());
            _gossipNodes.append(node);
        }
    } else {
        qDebug() << "Error while parsing settings file";
    }
}

NetworkSettings::NetworkSettings(NetworkSettings& orig) {
    _host = orig.getHost();
    _port = orig.getPort();
    _gossipInterval = orig.getGossipInterval();
    _cleanupInterval = orig.getCleanupInterval();
    _gossipNodes = orig.getGossipNodes();
    
}

NetworkSettings::~NetworkSettings() {
}

QList<QPair<QString, qint16> > NetworkSettings::getGossipNodes() {
    return _gossipNodes;
}


void NetworkSettings::setCleanupInterval(qint32 _cleanupInterval) {
    this->_cleanupInterval = _cleanupInterval;
}

qint32 NetworkSettings::getCleanupInterval() const {
    return _cleanupInterval;
}

void NetworkSettings::setGossipInterval(qint32 _gossipInterval) {
    this->_gossipInterval = _gossipInterval;
}

qint32 NetworkSettings::getGossipInterval() const {
    return _gossipInterval;
}

void NetworkSettings::setPort(qint32 _port) {
    this->_port = _port;
}

qint32 NetworkSettings::getPort() const {
    return _port;
}

void NetworkSettings::setHost(QString host) {
    _host = host;
}
QString NetworkSettings::getHost() const {
    return _host;
}

