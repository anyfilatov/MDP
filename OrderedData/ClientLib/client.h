#ifndef CLIENT
#define CLIENT

#include <QString>
#include <QTcpSocket>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include "../Router/typerequest.h"

class Client: public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    template <typename K, typename V> int put(K key, V value, QString bucket = NULL);
    template <typename K, typename V> QList<V> get(K key);
    int remove(QString bucket);
    QList<QString> getRingHosts();
    void initialOuterJoinRequest(QString idAddress);

protected:
    QTcpSocket *socket = NULL;
    QList<QString> hosts;
    int port;

    void writeMsg(QJsonObject msg);
    QJsonObject readMsg();
    QJsonObject deserialization(QByteArray data);
    QByteArray serialization(QJsonObject json);
    QByteArray createMessage(QJsonObject json);
    QJsonObject parseMessage(QByteArray data);
    int openConnect();

private slots:
    void disconnected();
};

template <typename K, typename V>
int Client::put(K key, V value, QString bucket){
    openConnect();

    QJsonObject jsonReq;
    QJsonObject jsonResp;

    jsonReq.insert("type", PUT);\
    jsonReq.insert("key", key);
    jsonReq.insert("value", value);

    if(bucket != NULL){
        jsonReq.insert("bucket", bucket);
    }

    writeMsg(jsonReq);
    jsonResp = readMsg();

    return jsonResp.value("status").toInt();
}

template <typename K, typename V>
QList<V> Client::get(K key){
    openConnect();

    QJsonObject jsonReq;
    QJsonObject jsonResp;

    jsonReq.insert("type", GET);\
    jsonReq.insert("key", key);

    writeMsg(jsonReq);
    jsonResp = readMsg();

    return jsonResp.value("value").toString().split(',');

}

#endif // CLIENT
