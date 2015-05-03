#include "connect.h"
#include "router.h"
#include "typerequest.h"

Connect::Connect(QMap<QString, QString> *map){
    this->rbtree = map;
}

void Connect::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    QByteArray msgReq;
    QByteArray msgResp;
    QJsonObject jsonReq;
    QJsonObject jsonResp;

    socket.setSocketDescriptor(socketDescriptor);
    socket.waitForReadyRead();
    while (socket.bytesAvailable()) {
        msgReq.append(socket.readAll());
    }

    jsonReq = parseMessage(msgReq);
    qDebug() << jsonReq;

    jsonResp = handleRequest(jsonReq);
    msgResp = createMessage(jsonResp);
    qDebug() << jsonResp;

    socket.write(msgResp);
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}

QJsonObject Connect::deserialization(QByteArray data){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(data);
    return jdoc.object();
}

QByteArray Connect::serialization(QJsonObject json){
    QJsonDocument jdoc(json);
    return jdoc.toJson();
}

QByteArray Connect::createMessage(QJsonObject json){
    QByteArray data = serialization(json);
    qDebug() << "data: " + data;

    QByteArray message;
    message.append(data.size()+1);
    message.append(data);
    return message;
}

QJsonObject Connect::parseMessage(QByteArray data){
    return deserialization(data.mid(1));
}

QJsonObject Connect::handleRequest(QJsonObject json){
    switch (json.find("type").value().toInt()) {
    case PUT:
        return handlePut(json);
    case GET:
        return handleGet(json);
    case DEL:
        return handleDelete(json);
    case RINGCECK:
        return handleRingCheck(json);
    case OUTERJOIN:
        return handleOuterJoin(json);
    default:
        break;
    }
}

QJsonObject Connect::handlePut(QJsonObject json){    
    QJsonObject jsonResp;
    if(json.contains("bucket")){
        rbtree->insert(json.find("bucket").value().toString() + "_keys", json.find("key").value().toString());
    }
    rbtree->insert(json.find("key").value().toString(),json.find("value").value().toString());
    jsonResp.insert("status", "OK");
    return jsonResp;
}

QJsonObject Connect::handleGet(QJsonObject json){
    QJsonObject jsonResp;
    QString value = rbtree->find(json.find("key").value().toString()).value();
    jsonResp.insert("key", json.find("key").value().toString());
    jsonResp.insert("value", value);
    jsonResp.insert("status", "OK");
    return jsonResp;
}

QJsonObject Connect::handleDelete(QJsonObject json){
    QJsonObject jsonResp;
    rbtree->empty();
    jsonResp.insert("status", "OK");
    return jsonResp;
}

QJsonObject Connect::handleRingCheck(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("ring", "127.0.0.1,127.0.0.2");
    return jsonResp;
}

QJsonObject Connect::handleOuterJoin(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("status", "OK");
    return jsonResp;
}

