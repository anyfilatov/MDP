#include "connect.h"
#include "router.h"
#include "typerequest.h"
#include "RouterClient/routerclient.h"


Connect::Connect(HashRing *ring, iCache<QString, QString> *map, QObject *parent): QObject(parent){
    _ring = ring;
    _rbtree = map;
}

void Connect::run()
{
    if(!socketDescriptor) return;
    _socket = new QTcpSocket();
    _socket->setSocketDescriptor(socketDescriptor);

    QByteArray msgReq;
    QByteArray msgResp;
    QJsonObject jsonReq;
    QJsonObject jsonResp;
    while (_socket->waitForReadyRead(10000)) {
        while (_socket->bytesAvailable()) {
            qDebug() << _socket->bytesAvailable();
            msgReq.append(_socket->readAll());
        }

        if (msgReq.size() == 0) {
            continue;
        }
        jsonReq = deserialize(msgReq);
        qDebug() << jsonReq;

        jsonResp = handleRequest(jsonReq);
        msgResp = serialize(jsonResp);
        qDebug() << jsonResp;

        _socket->write(msgResp);
        _socket->flush();
        _socket->waitForBytesWritten();

        msgReq.clear();
        msgResp.clear();
    }
    qDebug() << "Closed";
    _socket->disconnectFromHost();
}

QJsonObject Connect::deserialize(QByteArray data){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(data);
    return jdoc.object();
}

QByteArray Connect::serialize(QJsonObject json){
    QJsonDocument jdoc(json);
    return jdoc.toBinaryData();
}


QJsonObject Connect::handleRequest(QJsonObject json){
    switch (json.find("type").value().toInt()) {
    case PUT:
        return handlePut(json);
    case REPLACE:
        return handleReplace(json);
    case GET:
        return handleGet(json);
    case DEL:
        return handleRemove(json);
    case DEL_BUCKET:
        return handleRemoveBucket(json);
    case RINGCECK:
        return handleRingCheck(json);
    case OUTERJOIN:
        return handleRingJoin(json);
    default:
        break;
    }
}

QJsonObject Connect::handlePut(QJsonObject json){    
    QJsonObject jsonResp;

    QList<Node*> nodes = _ring->findNodes(json.value("key").toString());
    RouterClient client;
    return client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());

}

QJsonObject Connect::handleReplace(QJsonObject json)
{

}

QJsonObject Connect::handleGet(QJsonObject json){
    QJsonObject jsonResp;
    QString value = NULL;
    if (value != NULL){
        jsonResp.insert("key", json.find("key").value().toString());
        jsonResp.insert("value", value);
        jsonResp.insert("status", "OK");
    }
    else{
        jsonResp.insert("status", 404);
    }
    return jsonResp;
}

QJsonObject Connect::handleRemoveBucket(QJsonObject json)
{

}

QJsonObject Connect::handleRemove(QJsonObject json){
    QJsonObject jsonResp;
    int code = 0;
    if (code == 1) {
        jsonResp.insert("status", 200);
    } else{
        jsonResp.insert("status", 404);
    }
    return jsonResp;
}

QJsonObject Connect::handleRingCheck(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("ring", "127.0.0.1,127.0.0.2");
    return jsonResp;
}

QJsonObject Connect::handleRingJoin(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("status", 200);
    return jsonResp;
}

Connect::~Connect(){
    _socket->close();
    delete _socket;
}
