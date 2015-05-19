#include <QDataStream>

#include "remoteclient.h"
#include "Exception/exception.h"

RemoteClient::RemoteClient(int maxBufferSize, QString settingsFileName, QObject *parent)
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

RemoteClient::~RemoteClient()
{

}

RemoteClient::RemoteClient(int maxBufferSize, QObject *parent)
    : MAX_BUFFER_SIZE(maxBufferSize)
    , WAIT_TIME_MLS(5000)
{
    _hosts.append("127.0.0.1:12450");
}

int RemoteClient::put(QString key, QString value, QString bucket)
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
    if (_buffer.size() > MAX_BUFFER_SIZE)
        flush();
    return 0; // реально нужно int возвращать? может просто void?
}

void RemoteClient::flush()
{
    if (_buffer.isEmpty()) {
        return;
    }
    QJsonObject packet;
    packet.insert("type", PUT);
    packet.insert("packet", QJsonValue(_buffer));
    write(packet);
}

bool RemoteClient::isConnected()
{
    if (_socket->state() == QAbstractSocket::ConnectedState)
        return true;
    return false;
}

void RemoteClient::disconnect()
{
    if (!isConnected())
        return;
    _socket->disconnectFromHost();
    _socket->close();
    delete _socket;
}

void RemoteClient::openConnection()
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

QStringList RemoteClient::get(QString key, QString bucket) {

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

QStringList RemoteClient::getBucketKeys(QString bucket) {
  QJsonObject jsonReq;
  QJsonObject jsonResp;
  jsonReq.insert("type", GET);
  jsonReq.insert("key", bucket+"_keys");

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

int RemoteClient::remove(QString key, QString bucket) {
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

int RemoteClient::removeOne(QString key, QString value, QString bucket) {
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

QStringList RemoteClient::getRingHosts() {
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

void RemoteClient::joinToRing(QString who, QStringList ring) {
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

void RemoteClient::write(const QJsonObject &packet)
{
    QByteArray rawData = QJsonDocument(packet).toBinaryData();

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in << (quint32)rawData.size();
    in.writeRawData(rawData.constData(), rawData.size());

    openConnection();
    _socket->write(block);
    _socket->flush();
    _socket->waitForBytesWritten();
}

QJsonObject RemoteClient::read()
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

            char * buf = new char [packetSize];
            out.readRawData(buf, packetSize);
            response = QByteArray::fromRawData(buf, packetSize);
            delete [] buf;
            packetRecieved = true;
            
        }
    } else {
        throw new ServerUnavailableException();
    }

    return QJsonDocument::fromBinaryData(response).object();
}
