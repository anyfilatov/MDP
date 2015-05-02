#ifndef CLIENT
#define CLIENT

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include "typerequest.h"

class Client {
public:
    Client();
    template <typename K, typename V> virtual void put(K key, V value, QString bucket = NULL);
    template <typename K, typename V> virtual QList<V> get(K key);
    int virtual remove(QString bucket);
    QList<QString> virtual getRingHosts();
    void virtual initialOuterJoinRequest(QString idAddress);

protected:
    QTcpSocket *socket;
    QList<QString> hosts;
    int port;

    QString connectToServer();
    QJsonObject deserialization(QByteArray data);
    QByteArray serialization(QJsonObject json);
    QByteArray createMessage(QJsonObject json);
    QJsonObject parseMessage(QByteArray data);
};

Client::Client()
{
    QSettings *config = new QSettings("settings.conf",QSettings::NativeFormat);
    int portHost = config->value("CacheServerHost/portHost").toInt();
    QStringList ipsHost = config->value("CacheServerHost/ipsHost").toString().split(",");
    this->port = portHost;
    this->hosts = ipsHost;
    qsrand (QDateTime::currentMSecsSinceEpoch());
}

QString Client::connectToServer(){
    return hosts.at(qrand() % hosts.size());
}

QJsonObject Client::deserialization(QByteArray data){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(data);
    return jdoc.object();
}

QByteArray Client::serialization(QJsonObject json){
    QJsonDocument jdoc(json);
    return jdoc.toJson();
}

QByteArray Client::createMessage(QJsonObject json){
    QByteArray data = serialization(json);
    qDebug() << "data: " + data;

    QByteArray message;
    message.append(data.size()+1);
    message.append(data);
    return message;
}

QJsonObject Client::parseMessage(QByteArray data){
    return deserialization(data.mid(1));
}

template <typename K, typename V>
void Client::put(K key, V value, QString bucket){
    QString ip = connectToServer();
    qDebug() << ip;

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);

    if (!socket->waitForConnected(1000)){
        qDebug("Can't connect");

    }
    else {
        QJsonObject jsonReq;
        QJsonObject jsonResp;
        QByteArray msgReq;
        QByteArray msgResp;

        jsonReq.insert("type", PUT);\
        jsonReq.insert("key", key);
        jsonReq.insert("value", value);

        if(bucket != NULL){
            jsonReq.insert("bucket", bucket);
        }

        msgReq = createMessage(jsonReq);
        socket->write(msgReq);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        int sizeMsg;
        while (socket->bytesAvailable()) {
            sizeMsg = socket->bytesAvailable();
            msgResp = socket->readAll();
        }
        socket->close();

        jsonResp = parseMessage(msgResp);
    }
}

template <typename K, typename V>
QList<V> Client::get(K key){
    QString ip = connectToServer();
    qDebug() << ip;

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);

    if (!socket->waitForConnected(1000)){
        qDebug("Can't connect");
    }
    else {
        QJsonObject jsonReq;
        QJsonObject jsonResp;
        QByteArray msgReq;
        QByteArray msgResp;

        jsonReq.insert("type", GET);\
        jsonReq.insert("key", key);

        msgReq = createMessage(jsonReq);
        socket->write(msgReq);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        int sizeMsg;
        while (socket->bytesAvailable()) {
            sizeMsg = socket->bytesAvailable();
            msgResp = socket->readAll();
        }
        socket->close();

        jsonResp = parseMessage(msgResp);
        return jsonResp.value("value").toVariant().value;
    }
}

int Client::remove(QString bucket){
    QString ip = connectToServer();
    qDebug() << ip;

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);

    if (!socket->waitForConnected(1000)){
        qDebug("Can't connect");
    }
    else {
        QJsonObject jsonReq;
        QJsonObject jsonResp;
        QByteArray msgReq;
        QByteArray msgResp;

        jsonReq.insert("type", DEL);\
        jsonReq.insert("bucket", bucket);

        msgReq = createMessage(jsonReq);
        socket->write(msgReq);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        int sizeMsg;
        while (socket->bytesAvailable()) {
            sizeMsg = socket->bytesAvailable();
            msgResp = socket->readAll();
        }
        socket->close();

        jsonResp = parseMessage(msgResp);
        return jsonResp.value("status").toInt();
    }
}

QList<QString> Client::getRingHosts(){
    QString ip = connectToServer();
    qDebug() << ip;

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);

    if (!socket->waitForConnected(1000)){
        qDebug("Can't connect");
    }
    else {
        QJsonObject jsonReq;
        QJsonObject jsonResp;
        QByteArray msgReq;
        QByteArray msgResp;

        jsonReq.insert("type", RINGCECK);

        msgReq = createMessage(jsonReq);
        socket->write(msgReq);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        int sizeMsg;
        while (socket->bytesAvailable()) {
            sizeMsg = socket->bytesAvailable();
            msgResp = socket->readAll();
        }
        socket->close();

        jsonResp = parseMessage(msgResp);
        return jsonResp.value("hosts").toString().split(',');
    }
}

void Client::initialOuterJoinRequest(QString idAddress){
    QString ip = connectToServer();
    qDebug() << ip;

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);

    if (!socket->waitForConnected(1000)){
        qDebug("Can't connect");
    }
    else {
        QJsonObject jsonReq;
        QJsonObject jsonResp;
        QByteArray msgReq;
        QByteArray msgResp;

        jsonReq.insert("type", OUTERJOIN);
        jsonReq.insert("address", idAddress);

        msgReq = createMessage(jsonReq);
        socket->write(msgReq);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        int sizeMsg;
        while (socket->bytesAvailable()) {
            sizeMsg = socket->bytesAvailable();
            msgResp = socket->readAll();
        }
        socket->close();

        jsonResp = parseMessage(msgResp);
    }
}

#endif // CLIENT
