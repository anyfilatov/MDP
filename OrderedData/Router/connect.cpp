#include "connect.h"
#include "router.h"
#include "typerequest.h"
#include "RouterClient/routerclient.h"

Connect::Connect(iCache<QString, QString> *map, QObject *parent): QObject(parent){

}

void Connect::run()
{
    if(!socketDescriptor) return;
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    QByteArray msgReq;
    QByteArray msgResp;
    QJsonObject jsonReq;
    QJsonObject jsonResp;
    while (socket->waitForReadyRead(10000)) {
        while (socket->bytesAvailable()) {
            qDebug() << socket->bytesAvailable();
            msgReq.append(socket->readAll());
        }

        if (msgReq.size() == 0) {
            continue;
        }
        jsonReq = parseMessage(msgReq);
        qDebug() << jsonReq;

        jsonResp = handleRequest(jsonReq);
        msgResp = createMessage(jsonResp);
        qDebug() << jsonResp;

        socket->write(msgResp);
        socket->flush();
        socket->waitForBytesWritten();

        msgReq.clear();
        msgResp.clear();
    }
    qDebug() << "Closed";
    socket->disconnectFromHost();
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
    jsonResp.insert("status", 200);
    //return jsonResp;
    RouterClient client;
    return client.doRequestToOtherRouter(json, "192.168.1.5", 12340, true);
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

QJsonObject Connect::handleDelete(QJsonObject json){
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

QJsonObject Connect::handleOuterJoin(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("status", 200);
    return jsonResp;
}

Connect::~Connect(){
    socket->close();
    delete socket;
}
