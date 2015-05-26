#include <QDataStream>

#include "client.h"
#include "Exception/exception.h"

Client::Client(int maxBufferSize, QString settingsFileName, QObject *parent)
    : MAX_BUFFER_SIZE(maxBufferSize)
    , WAIT_TIME_MLS(5000)
{
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
    settingsFile.close();
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

Client::~Client()
{

}

Client::Client(int maxBufferSize, QObject *parent)
    : MAX_BUFFER_SIZE(maxBufferSize)
    , WAIT_TIME_MLS(5000)
{
    _hosts.append("127.0.0.1:12450");
}

int Client::put(QString key, QString value, QString bucket)
{
    QJsonObject json;
    if(bucket == NULL){
        json.insert("key", key);
        json.insert("value", value);
        QJsonValue jsonValue(json);
        _buffer.append(jsonValue);
    } else {
        put(bucket+"#"+key, value);
        put(bucket+"_keys", key);
    }
    if (_buffer.size() >= MAX_BUFFER_SIZE) {
        flush();
    }
    return 0; // реально нужно int возвращать? может просто void?
}

void Client::flush()
{
    qDebug() << "flush=" << _buffer.size();
    if (_buffer.isEmpty()) {
        return;
    }
    QJsonObject packet;
    packet.insert("type", PUT);
    packet.insert("packet", QJsonValue(_buffer));

    write(packet);

    while (!_buffer.isEmpty()) {
        _buffer.removeLast();
    }
    qDebug() << "size=" << _buffer.size();
    disconnectFromHost();
}

bool Client::isConnected()
{
    if (_socket->state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void Client::disconnectFromHost()
{
    if (!isConnected())
        return;
    _socket->disconnectFromHost();
    _socket->waitForDisconnected();
    _socket->close();
    delete _socket;
    _socket = NULL;
}

void Client::openConnection()
{
    if (_socket == NULL) {
      _socket = new QTcpSocket();
    }

    if (_socket->state() == QAbstractSocket::UnconnectedState){
        QString host = _hosts.at(qrand() % _hosts.size());
        qDebug() << "Trying to connect to: " << host;

        _socket->connectToHost(QHostAddress(host.split(":").first()),
                               host.split(":").back().toInt());

        if (!_socket->waitForConnected(1000)) {
            int index = _hosts.indexOf(host);
            host = _hosts.at(index % _hosts.size());
            _socket->connectToHost(QHostAddress(host.split(":").first()),
                                   host.split(":").back().toInt());
            _hosts = this->getRingHosts();
        }
    }
}

QStringList Client::get(QString key, QString bucket) {

    QJsonObject jsonReq;
    QJsonObject jsonResp;
    jsonReq.insert("type", GET);
    if(bucket == NULL){
        jsonReq.insert("key", key);
    } else {
        jsonReq.insert("key", bucket+"#"+key);
    }
    write(jsonReq);
    jsonResp = read();

  QStringList response;
  if (jsonResp.value("status").toInt() == StatusCode::OK) {
    for (QJsonValue value : jsonResp.value("values").toArray()) {
      response << value.toString();
    }
  } else if (jsonResp.value("status").toInt() == StatusCode::NOT_FOUND) {
    throw NotFoundValueException();
  } else {
    throw ServerUnavailableException();
  }
  return response;
}

QStringList Client::getBucketKeys(QString bucket) {
  QJsonObject jsonReq;
  QJsonObject jsonResp;
  jsonReq.insert("type", GET);
  jsonReq.insert("key", bucket+"_keys");

  write(jsonReq);
  jsonResp = read();

  QStringList response;
  qDebug() << "jsonResp.value(values).toArray()=" << jsonResp.value("values").toArray().size();
  if (jsonResp.value("status").toInt() == StatusCode::OK) {
    for (QJsonValue value : jsonResp.value("values").toArray()) {
      response << value.toString();
    }
  } else if (jsonResp.value("status").toInt() == StatusCode::NOT_FOUND) {
    throw NotFoundValueException();
  } else {
    throw ServerUnavailableException();
  }
  return response;
}

int Client::remove(QString key, QString bucket) {
  QJsonObject jsonReq;
  QJsonObject jsonResp;

  if(bucket == NULL){
      jsonReq.insert("type", DEL);
      jsonReq.insert("key", key);
      write(jsonReq);
      jsonResp = read();
      return jsonResp.value("status").toInt();
  } else {
      int codeKey = remove(bucket+"#"+key);
      int codeBucket = removeOne(bucket+"_keys", key);
      if (codeKey != 0 || codeBucket != 0){
          throw ServerUnavailableException();
      }
      return 0;
  }
}

int Client::removeOne(QString key, QString value, QString bucket) {
    QJsonObject jsonReq;
    QJsonObject jsonResp;

    jsonReq.insert("type", DEL_ONE);
    if(bucket == NULL){
        jsonReq.insert("key", key);
    } else {
        jsonReq.insert("key", bucket+"#"+key);
    }
    jsonReq.insert("value", value);
    write(jsonReq);
    jsonResp = read();
    return jsonResp.value("status").toInt();
}

QStringList Client::getRingHosts() {
  QJsonObject jsonReq;
  QJsonObject jsonResp;
  jsonReq.insert("type", RINGCECK);
  write(jsonReq);
  jsonResp = read();
  QStringList response;
  if (jsonResp.value("status").toInt() == StatusCode::SERVER_UNAVAILABLE) {
    throw ServerUnavailableException();
  } else {
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

  QByteArray rawData = QJsonDocument(jsonReq).toBinaryData();

  QByteArray block;
  QDataStream in(&block, QIODevice::WriteOnly);
  in.setVersion(QDataStream::Qt_5_0);
  in << (quint32)rawData.size();
  in.writeRawData(rawData.constData(), rawData.size());

  socket->write(block);
  socket->flush();
  socket->disconnectFromHost();
  socket->close();

  delete socket;
}

void Client::write(const QJsonObject &packet)
{
//    qDebug() << "sending " << packet;
    QByteArray rawData = QJsonDocument(packet).toJson();

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in << rawData;

    qDebug() << "Sending " << block.size() << "bytes";
    openConnection();
    _socket->write(block);
    _socket->flush();
    _socket->waitForBytesWritten();
}

QJsonObject Client::read()
{
    QByteArray response;

    quint32 packetSize = 0;
    bool packetRecieved = false;

    QDataStream out(_socket);
    out.setVersion(QDataStream::Qt_5_0);

    if (_socket->waitForReadyRead(WAIT_TIME_MLS)) {
        while(!packetRecieved) {
            if (packetSize == 0) {
                if (_socket->bytesAvailable() < (quint32) sizeof(quint32))
                    continue;
                out >> packetSize;
            }

            if (_socket->bytesAvailable() < packetSize)
                continue;
            qDebug() << "packetSize=" << packetSize;
            char * buf = new char [packetSize];
            out.readRawData(buf, packetSize);
            response = QByteArray::fromRawData(buf, packetSize);
            delete [] buf;
            packetRecieved = true;
            
        }
    } else {
        throw new ServerUnavailableException();
    }
    qDebug() << "response.size=" << response.size();
    return QJsonDocument::fromJson(response).object();
}
