#include <QHostAddress>
#include <QtDebug>
#include "client.h"

Client::Client(int maxBufferSize, QString settingsFileName) :
    MAX_BUFFER_SIZE(maxBufferSize)
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

void Client::put(const QString &key, const QString &value)
{
    QJsonObject json;
    json.insert("type", PUT);
    json.insert("key", key);
    json.insert("value", value);
    QJsonValue jsonValue(json);
    _buffer.append(jsonValue);
    if (_buffer.size() >= MAX_BUFFER_SIZE)
        flush();
}

void Client::put(const QString &key, const QString &value, const QString &bucket)
{
    put(bucket+"#"+key, value);
    putRewriteValue(bucket+"_keys", key);
}

QStringList Client::get(const QString& key)
{
    QJsonObject reqJson, respJson;
    reqJson.insert("type", GET);
    reqJson.insert("key", key);

    connectToHost();
    write(reqJson);
    respJson = read();

    QStringList response;
    if (respJson["status"].toInt() == OK) {
        for (QJsonValue value : respJson["values"].toArray()) {
            response << value.toString();
        }
    } else if (respJson["status"].toInt() == NOT_FOUND) {
        throw NotFoundValueException();
    } else {
        throw ServerUnavailableException();
    }

    return response;
}

QStringList Client::get(const QString& key, const QString& bucket)
{
    return get(bucket + "#" + key);
}

QStringList Client::getBucketKeys(const QString &bucket)
{
    return get(bucket+"_keys");
}

void Client::remove(const QString &key)
{
    QJsonObject reqJson;
    reqJson.insert("type", DEL);
    reqJson.insert("key", key);
    _buffer.append(reqJson);
    if (_buffer.size() >= MAX_BUFFER_SIZE){
        flush();
    }
}

void Client::remove(const QString &key, const QString &bucket)
{
    remove(bucket + "#" + key);
    removeValue(bucket + "_keys", key);
}

QStringList Client::getRingHosts()
{
    QJsonObject reqJson, respJson;
    reqJson.insert("type", RINGCECK);

    connectToHost();
    write(reqJson);
    respJson = read();

    QStringList response;
    if (respJson["status"].toInt() == SERVER_UNAVAILABLE) {
        throw ServerUnavailableException();
    } else {
        for (QJsonValue value : respJson["hosts"].toArray()) {
            response << value.toString();
        }
    }
    return response;
}

void Client::joinToRing(const QString &who, const QStringList &ring)
{
    QJsonObject reqJson;
    reqJson.insert("type", OUTERJOIN);
    reqJson.insert("ring", QJsonValue(QJsonArray::fromStringList(ring)));

    QTcpSocket* socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(who.split(":").first()), who.split(":").back().toInt());
    if (_socket.waitForConnected(AbstractClient::MAX_WAIT_TIME_MLS)) {
        QByteArray rawData = QJsonDocument(reqJson).toBinaryData();

        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out << (quint32)0;
        out << rawData;
        out.device()->seek(0);
        out << (quint32)(packet.size() - sizeof(quint32));

        if (socket->isWritable()) {
            socket->write(packet);
            socket->waitForBytesWritten(AbstractClient::MAX_WAIT_TIME_MLS);
            socket->flush();
            socket->disconnect();
            delete socket;
        }
    } else {
        throw ServerUnavailableException();
    }
}

void Client::flush()
{
    if (_buffer.isEmpty()) {
        return;
    }
    QJsonObject packet;
    packet.insert("type", BATCH);
    packet.insert("packet", _buffer);
    if( !connectToHost() ){
        throw ServerUnavailableException();
    };
    write(packet);

    while (!_buffer.isEmpty()) {
        _buffer.removeLast();
    }
}

void Client::putRewriteValue(const QString &key, const QString &value)
{
    QJsonObject json;
    json.insert("type", PUT_REWRITE);
    json.insert("key", key);
    json.insert("value", value);
    _buffer.append(json);
    if (_buffer.size() >= MAX_BUFFER_SIZE){
        flush();
    }
}

void Client::removeValue(const QString &key, const QString &value)
{
    QJsonObject json;
    json.insert("type", DEL_ONE);
    json.insert("key", key);
    json.insert("value", value);
    _buffer.append(json);
    if (_buffer.size() >= MAX_BUFFER_SIZE){
        flush();
    }
}

bool Client::connectToHost()
{
    if (_socket.state() == QAbstractSocket::UnconnectedState) {
        bool isConnected =  false;
        int retryConnectionCount = 3;
        while(true){
            QString host = _hosts.at(qrand() % _hosts.size());
            isConnected = AbstractClient::connectToHost(host.split(':').first(), host.split(':').back().toInt());
            if(isConnected){
                return true;
            }
            else {
                if (retryConnectionCount == 0)
                    return false;
                retryConnectionCount--;
            }
        }
    }
    return true;
}
