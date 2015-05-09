#include "connect.h"
#include "router.h"
#include "typerequest.h"
#include "RouterClient/routerclient.h"
#include "Router/StatusCodes.h"


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
    if (json.contains("not_forwards_requests") && json.value("not_forwards_requests").toBool()) {
        if(json.contains("value")){
            //_rbtree->insert(json.value("key").toString(), json.value("value").toString());
        } else {
            //_rbtree->insert(json.value("key").toString(), json.value("values").toString());
        }
        jsonResp.insert("status", StatusCode::OK);
    } else{
        QList<Node*> nodes;
        if(json.contains("bucket")){
            nodes = _ring->findNodes(json.value("bucket").toString().append(json.value("key").toString()));
            RouterClient client;
            QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
            QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
            StatusCode codeBucket = (StatusCode)client.put(json.value("bucket").toString().append("_keys"), json.value("key").toString());
            StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
            StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();
            if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK || codeBucket == StatusCode::OK) {
                jsonResp.insert("status", StatusCode::OK);
            } else {
                jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
            }
        } else {
            nodes = _ring->findNodes(json.value("key").toString());
            RouterClient client;
            QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
            QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
            StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
            StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();
            if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
                jsonResp.insert("status", StatusCode::OK);
            } else {
                jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
            }
        }
    }
    return jsonResp;
}

QJsonObject Connect::handleReplace(QJsonObject json)
{
    QJsonObject jsonResp;
    if (json.contains("not_forwards_requests") && json.value("not_forwards_requests").toBool()) {
        //_rbtree->replace(json.value("key").toString(), json.value("values").toString());
        jsonResp.insert("status", StatusCode::OK);
    } else{
        QList<Node*> nodes;
        if(json.contains("bucket")){
            nodes = _ring->findNodes(json.value("bucket").toString().append(json.value("key").toString()));
        } else {
            nodes = _ring->findNodes(json.value("key").toString());
        }

        RouterClient client;
        QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
        QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
        StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
        StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();
        if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
            jsonResp.insert("status", StatusCode::OK);
        } else {
            jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        }
    }

    return jsonResp;
}

QJsonObject Connect::handleGet(QJsonObject json){
    QJsonObject jsonResp;
    if (json.contains("not_forwards_requests") && json.value("not_forwards_requests").toBool()) {
        QStringList values;// = _rbtree->search(json.value("key").toString());
        jsonResp.insert("key", json.value("key"));
        jsonResp.insert("values", QJsonValue(QJsonArray::fromStringList(values)));
        jsonResp.insert("status", StatusCode::OK);
    } else{
        QList<Node*> nodes;
        if(json.contains("bucket")){
            nodes = _ring->findNodes(json.value("bucket").toString().append(json.value("key").toString()));
        } else {
            nodes = _ring->findNodes(json.value("key").toString());
        }

        RouterClient client;
        QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
        QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
        StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
        StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();

        if (codePrimary == StatusCode::OK && codeReplica == StatusCode::OK) {
            if(jsonPrimaryResp.value("values").toArray().size() > jsonReplicaResp.value("values").toArray().size())
                jsonResp.insert("values", jsonPrimaryResp.value("values"));
            else
                jsonResp.insert("values", jsonPrimaryResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else if (codePrimary == StatusCode::OK) {
            jsonResp.insert("values", jsonPrimaryResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else if (codeReplica == StatusCode::OK){
            jsonResp.insert("values", jsonReplicaResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else {
            jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        }
    }
    return jsonResp;
}

QJsonObject Connect::handleRemoveBucket(QJsonObject json)
{
    QJsonObject jsonResp;
    if (json.contains("not_forwards_requests") && json.value("not_forwards_requests").toBool()) {
        RouterClient client;
        QStringList values = client.get(json.value("bucket").toString().append("_keys"));
        for(QString value: values){
            client.remove(value);
        }
        StatusCode delCode = (StatusCode)_rbtree->remove(json.value("bucket").toString());
        jsonResp.insert("status", delCode);
    } else{
        QList<Node*> nodes;
        nodes = _ring->findNodes(json.value("bucket").toString().append("_keys"));
        RouterClient client;
        QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
        QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
        StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
        StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();

        if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
            jsonResp.insert("status", StatusCode::OK);
        } else {
            jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        }
    }
}

QJsonObject Connect::handleRemove(QJsonObject json){
    QJsonObject jsonResp;
    if (json.contains("not_forwards_requests") && json.value("not_forwards_requests").toBool()) {
        StatusCode delCode = (StatusCode)_rbtree->remove(json.value("key").toString());
        jsonResp.insert("status", delCode);
    } else{
        QList<Node*> nodes;
        nodes = _ring->findNodes(json.value("key").toString());
        RouterClient client;
        QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort());
        QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(json, nodes[0]->getHost(), nodes[0]->getPort(), true);
        StatusCode codePrimary = (StatusCode)jsonPrimaryResp.value("status").toInt();
        StatusCode codeReplica = (StatusCode)jsonReplicaResp.value("status").toInt();

        if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
            jsonResp.insert("status", StatusCode::OK);
        } else {
            jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        }
    }
    return jsonResp;
}

QJsonObject Connect::handleRingCheck(QJsonObject json){
    QJsonObject jsonResp;
    QStringList hosts;
    QList<Node*> nodes = _ring->getAllMember();
    if (nodes.size() < 1) {
        jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        return jsonResp;
    }
    for (Node* node : nodes){
        hosts.push_back(node->getAddress());
    }
    jsonResp.insert("ring", QJsonValue(QJsonArray::fromStringList(hosts)));
    jsonResp.insert("status", StatusCode::OK);
    return jsonResp;
}

QJsonObject Connect::handleRingJoin(QJsonObject json){
    QJsonObject jsonResp;
    jsonResp.insert("status", StatusCode::OK);
    return jsonResp;
}

Connect::~Connect(){
    _socket->close();
    delete _socket;
}
