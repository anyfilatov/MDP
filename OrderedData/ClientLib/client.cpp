#include "client.h"
#include "Exception/exception.h"

Client::Client(QString settingsFileName, QObject* parent) : QObject(parent) {
  QFile settingsFile(settingsFileName);
  if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    throw std::runtime_error("Couldn't open file");

  QJsonParseError* parseError = NULL;
  QJsonDocument document =
      QJsonDocument::fromJson(settingsFile.readAll(), parseError);
  if (parseError == NULL) {
    QJsonObject json = document.object();
    QJsonArray members = json.value("members").toArray();
    for (int i = 0; i < members.size(); i++) {
      QString member = members.at(i).toString();
      _hosts.append(member);
    }
  } else {
    throw std::runtime_error("Error while parsing settings file");
  }

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
    _socket->connectToHost(QHostAddress(host.split(":").first()),
                           host.split(":").back().toInt());

    if (!_socket->waitForConnected(1000)) {
      throw ConnectionIsNotCreatedException();
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
    throw ServerUnavailableException();
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
  if(jsonResp.value("status").toInt() == StatusCode::OK){

      for (QJsonValue value : jsonResp.value("values").toArray()) {
        response << value.toString();
      }
  } else if (jsonResp.value("status").toInt() == StatusCode::NOT_FOUND){
      throw NotFoundValueException();
  } else {
      throw ServerUnavailableException();
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
  if(jsonResp.value("status").toInt() == StatusCode::SERVER_UNAVAILABLE){
      throw ServerUnavailableException();
  } else{
      QStringList response;
      for (QJsonValue value : jsonResp.value("hosts").toArray()) {
        response << value.toString();
      }
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

Client::Client(QObject* parent) : QObject(parent) {}

void Client::disconnected() {
  qDebug() << "Server disconnected";
  qDebug() << _socket->state();
}
