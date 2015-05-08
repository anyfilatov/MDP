#include "client.h"

Client::Client(QObject* parent) : QObject(parent) {
  QList<QString> ipsHost;
  ipsHost.append("192.168.1.10");
  this->_hosts = ipsHost;
  qsrand(QDateTime::currentMSecsSinceEpoch());
}

int Client::openConnection() {
  if (_socket == NULL) {
    _socket = new QTcpSocket();
    connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  }

  if (_socket->state() == QAbstractSocket::UnconnectedState) {
    QString host = _hosts.at(qrand() % _hosts.size());
    qDebug() << "Open connect on " << host;
    _socket->connectToHost(QHostAddress(host.split(":").first()), host.split(":").back().toInt());

    if (!_socket->waitForConnected(1000)) {
      qDebug("Can't connect");
      return -1;
    }
  }
  return 0;
}

void Client::writeMsg(QJsonObject json) {
  QByteArray data = serialize(json);

  _socket->write(data);
  _socket->waitForBytesWritten();
}

QJsonObject Client::readMsg() {
  QByteArray msg;
  _socket->waitForReadyRead();
  while (_socket->bytesAvailable()) {
    msg = _socket->readAll();
  }

  if (msg.size() == 0) {
    msg.append("{\"status\": 500}");
  }

  return deserialize(msg);
}

QJsonObject Client::deserialize(QByteArray data) {
  QJsonDocument jdoc;
  jdoc = jdoc.fromJson(data);
  return jdoc.object();
}

QByteArray Client::serialize(QJsonObject json) {
  QJsonDocument jdoc(json);
  return jdoc.toBinaryData();
}

int Client::put(QString key, QString value, QString bucket) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", PUT);
  jsonReq.insert("key", key);
  jsonReq.insert("value", value);

  if (bucket != NULL) {
    jsonReq.insert("bucket", bucket);
  }

  writeMsg(jsonReq);
  jsonResp = readMsg();

  // TODO подумать над кодами ошибок/exception'ами
  return jsonResp.value("status").toInt();
}

int Client::put(QString key, QStringList values, QString bucket) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", PUT);
  jsonReq.insert("key", key);
  jsonReq.insert("values", QJsonValue(QJsonArray::fromStringList(values)));

  if (bucket != NULL) {
    jsonReq.insert("bucket", bucket);
  }

  writeMsg(jsonReq);
  jsonResp = readMsg();

  // TODO подумать над кодами ошибок/exception'ами
  return jsonResp.value("status").toInt();
}

int Client::replace(QString key, QStringList values, QString bucket) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", REPLACE);
  jsonReq.insert("key", key);
  jsonReq.insert("values", QJsonValue(QJsonArray::fromStringList(values)));

  if (bucket != NULL) {
    jsonReq.insert("bucket", bucket);
  }

  writeMsg(jsonReq);
  jsonResp = readMsg();

  // TODO подумать над кодами ошибок/exception'ами
  return jsonResp.value("status").toInt();
}

QStringList Client::get(QString key, QString bucket) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", GET);
  jsonReq.insert("key", key);

  if (bucket != NULL) {
    jsonReq.insert("bucket", bucket);
  }

  writeMsg(jsonReq);
  jsonResp = readMsg();

  QStringList response;
  for (QJsonValue value : jsonResp.value("values").toArray()) {
    response << value.toString();
  }

  return response;
}

int Client::remove(QString key) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", DEL);
  jsonReq.insert("key", key);

  writeMsg(jsonReq);
  jsonResp = readMsg();

  return jsonResp.value("status").toInt();
}

int Client::removeBucket(QString bucket) {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", DEL_BUCKET);
  jsonReq.insert("bucket", bucket);

  writeMsg(jsonReq);
  jsonResp = readMsg();

  return jsonResp.value("status").toInt();
}

QStringList Client::getRingHosts() {
  openConnection();

  QJsonObject jsonReq;
  QJsonObject jsonResp;

  jsonReq.insert("type", RINGCECK);

  writeMsg(jsonReq);
  jsonResp = readMsg();

  QStringList response;
  for (QJsonValue value : jsonResp.value("hosts").toArray()) {
    response << value.toString();
  }

  return response;
}

void Client::joinToRing(QString who, QStringList ring) {
  QTcpSocket* socket = new QTcpSocket();
  socket->connectToHost(QHostAddress(who.split(":").first()),
                        who.split(":").back().toInt());

  QJsonObject jsonReq;

  jsonReq.insert("type", OUTERJOIN);
  jsonReq.insert("ring", QJsonValue(QJsonArray::fromStringList(ring)));

  socket->write(serialize(jsonReq));
  socket->waitForBytesWritten();
  socket->disconnect();

  delete socket;
}

void Client::disconnected() {
  qDebug() << "Server disconnected";
  qDebug() << _socket->state();
}
