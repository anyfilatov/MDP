#include "node.h"
#include "NetworkSettings.h"
#include "networkmanager.h"
#include <QDebug>

Node::Node(QString host, qint16 port, qint32 heartbeat, qint32 cleanupInterval,
           NetworkManager& manager) {
  _host = host;
  _port = port;
  _heartbeat = heartbeat;
  _timer.setInterval(cleanupInterval);
  _timer.setSingleShot(true);

  connect(this, SIGNAL(heartbeatChanged()), this,
          SLOT(handleHeartbeatChanged()));
  connect(&_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
  connect(this, SIGNAL(notifyManager(QString)), &manager,
          SLOT(handleDeadNode(QString)));
}

Node::Node(QString host, qint16 port, qint32 heartbeat) {
  _host = host;
  _port = port;
  _heartbeat = heartbeat;
}

Node::Node(const Node& orig) {
  _host = orig.getHost();
  _port = orig.getPort();
  _heartbeat = orig.getHeartbeat();
  _timer.setInterval(orig.getTimer()->interval());
  _timer.setSingleShot(true);

  //    connect(this, SIGNAL(heartbeatChanged()), this,
  //    SLOT(handleHeartbeatChanged()));
  //    connect(_timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
  //    connect(this, SIGNAL(notifyManager(QString)), &manager,
  //    SLOT(handleDeadNode()));
}

QString Node::getHost() const { return _host; }

qint16 Node::getPort() const { return _port; }

qint32 Node::getHeartbeat() const { return _heartbeat; }

void Node::setHeartbeat(qint32 heartbeat) {
  this->_heartbeat = heartbeat;
  emit heartbeatChanged();
}

QString Node::getAddress() const {
  return _host + ":" + QString::number(_port);
}

const QTimer* Node::getTimer() const { return &_timer; }

void Node::startTimer() { _timer.start(); }

Node& Node::operator=(const Node& other) {
  _host = other.getHost();
  _port = other.getPort();
  _heartbeat = other.getHeartbeat();
  _timer.setInterval(other.getTimer()->interval());
  _timer.setSingleShot(true);
  return *this;
}

bool Node::operator==(const Node& other) const {
  return this->getAddress() == other.getAddress();
}

bool Node::operator<(const Node& other) const {
  return this->getAddress() < other.getAddress();
}

QJsonObject& Node::toJsonObject() const {
  QJsonObject* json = new QJsonObject();
  json->insert("host", QJsonValue(_host));
  json->insert("port", QJsonValue(_port));
  json->insert("heartbeat", QJsonValue(_heartbeat));
  return *json;
}

void Node::handleHeartbeatChanged() { _timer.start(); }

void Node::handleTimeout() { emit notifyManager(getAddress()); }

Node::~Node() {}
