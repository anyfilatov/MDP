#include <QVector>
#include <QtConcurrent/QtConcurrent>

#include "connection.h"
#include "router.h"
#include "ClientLib/typerequest.h"
#include "ClientLib/StatusCodes.h"
#include "Exception/serverunavailableexception.h"
#include "Cache/rbtree.h"
#include "RouterClient/routerclient.h"

Connection::Connection(qintptr handle, HashRing* ring, Cache<QString, QString>* map, QMutex *mutex, QString myAddress, QObject* parent)
    : QObject(parent) {
    _socketDescriptor = handle;
    _ring = ring;
    _rbtree = map;
    _mutex = mutex;
    _me = myAddress;
}

void Connection::run() {
    _socket = new QTcpSocket();
    if (!_socket->setSocketDescriptor(_socketDescriptor))
        return;
    while(isConnected()){
        if(_socket->bytesAvailable() > 0) {
            QJsonObject request = read();
            if (request["type"] == BATCH) {
                handleBatchRequest(request);
            } else {
                QJsonObject response = handleRequest(request);
                write(response);
            }
        } else {
            _socket->waitForReadyRead();
        }
    }
}

QJsonObject Connection::handleRequest(const QJsonObject &json)
{
    switch (json.find("type").value().toInt()) {
    case GET:
        return handleGet(json);
    case RINGCECK:
        return handleRingCheck();
    case OUTERJOIN:
        return handleRingJoin(json);
    case STATS:
        return handleStatistics();
    }
}

void Connection::handleBatchRequest(const QJsonObject &json)
{
    if (json.contains("not_forwards_request") && json["not_forwards_request"].toBool()) {
//        qDebug() << "Goint to insert/remove " << json["packet"].toArray().size() << " elements";
        for (QJsonValue value: json["packet"].toArray()) {
            QJsonObject object = value.toObject();
            switch (object["type"].toInt()) {
            case PUT: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                bool isReplica = object.value("isReplica").toBool();
                _mutex->lock();
                _rbtree->insert(key, value, false, isReplica);
                _mutex->unlock();
                break;
            }
            case PUT_REWRITE: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                bool isReplica = object.value("isReplica").toBool();
                _mutex->lock();
                _rbtree->insert(key, value, true, isReplica);
                _mutex->unlock();
                break;
            }
            case DEL: {
                QString key = object.value("key").toString();
                _mutex->lock();
                _rbtree->remove(key);
                _mutex->unlock();
                break;
            }
            case DEL_ONE: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                _mutex->lock();
                _rbtree->remove(key, value);
                _mutex->unlock();
                break;
            }
            default:
                break;
            }
        }
    } else {
        QList<Node> nodes;
        RBTree<QString, QJsonObject> temp;

        QJsonArray array = json.value("packet").toArray();
        qDebug() << "Recieved array size = " << array.size();
        qDebug() << "Count = " << _count++;

        for (QJsonValueRef kv : array) {
            nodes = _ring->findNodes(kv.toObject().value("key").toString());

            QJsonObject primary(kv.toObject());
            primary.insert("isReplica", false);

            QJsonObject replica(kv.toObject());
            replica.insert("isReplica", true);

            temp.insert(nodes[0].getAddress(), primary);
            temp.insert(nodes[1].getAddress(), replica);
        }

        QList<QString> list = temp.getKeys();

        RouterClient* client = new RouterClient();
        for (QString node : list) {
            QJsonObject proscessedJson;
            proscessedJson.insert("type", BATCH);
            QJsonArray values;
            for (QJsonObject obj : temp.search(node)) {
                values.append(obj);
            }
            proscessedJson.insert("packet", values);
//            qDebug() << "Sending packet to node: " << node;
//            qDebug() << "Me " << _me;
            if(node == _me) {
                proscessedJson.insert("not_forwards_request", true);
                handleBatchRequest(proscessedJson);
            } else {
                client->doRequestToOtherRouter(
                            proscessedJson, node.split(":").first(), node.split(":").last().toInt());
            }
        }
    }
}

QJsonObject Connection::handleGet(const QJsonObject &json)
{
    QJsonObject jsonResp;
    if (json.contains("not_forwards_request") &&
            json.value("not_forwards_request").toBool()) {
        _mutex->lock();
        QStringList values = _rbtree->search(json.value("key").toString());
        _mutex->unlock();
        jsonResp.insert("key", json.value("key"));
        jsonResp.insert("values", QJsonValue(QJsonArray::fromStringList(values)));
        jsonResp.insert("status", StatusCode::OK);
    } else {
        QList<Node> nodes;
        nodes = _ring->findNodes(json.value("key").toString());

        RouterClient client;
        QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(
                    json, nodes[0].getHost(), nodes[0].getPort());
        QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(
                    json, nodes[1].getHost(), nodes[1].getPort());
        StatusCode codePrimary =
                (StatusCode)jsonPrimaryResp.value("status").toInt();
        StatusCode codeReplica =
                (StatusCode)jsonReplicaResp.value("status").toInt();

        if (codePrimary == StatusCode::OK && codeReplica == StatusCode::OK) {
            if (jsonPrimaryResp.value("values").toArray().size() >
                    jsonReplicaResp.value("values").toArray().size())
                jsonResp.insert("values", jsonPrimaryResp.value("values"));
            else
                jsonResp.insert("values", jsonPrimaryResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else if (codePrimary == StatusCode::OK) {
            jsonResp.insert("values", jsonPrimaryResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else if (codeReplica == StatusCode::OK) {
            jsonResp.insert("values", jsonReplicaResp.value("values"));
            jsonResp.insert("status", StatusCode::OK);
        } else {
            jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        }
    }
    return jsonResp;
}

QJsonObject Connection::handleRingCheck()
{
    QJsonObject jsonResp;
    QStringList hosts;
    QList<Node*> nodes = _ring->getAllMember();
    if (nodes.size() < 1) {
        jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
        return jsonResp;
    }
    for (Node* node : nodes) {
        hosts.push_back(node->getAddress());
        delete node;
    }
    jsonResp.insert("hosts", QJsonValue(QJsonArray::fromStringList(hosts)));
    jsonResp.insert("status", StatusCode::OK);
    return jsonResp;
}

QJsonObject Connection::handleRingJoin(const QJsonObject &json)
{
    QJsonObject jsonResp;
    QStringList values = json.value("ring").toVariant().toStringList();
    QList<Node*> members = _ring->getAllMember();
    for (Node* node : members) {
        if (values.contains(node->getAddress())) {
            values.removeOne(node->getAddress());
        }
        delete node;
    }
    _ring->getManager()->addMembers(values);
    jsonResp.insert("status", StatusCode::OK);
    return jsonResp;
}

QJsonObject Connection::handleStatistics()
{
    QJsonObject jsonResp;
    jsonResp.insert("keys_count", 0);
    jsonResp.insert("red_nodes_count", 0);
    jsonResp.insert("black_nodes_count", 0);
    jsonResp.insert("primary_count", 0);
    jsonResp.insert("replica_count", 0);
    jsonResp.insert("height_count", 0);
    jsonResp.insert("hits_count", 0);
    jsonResp.insert("status", OK);
    return jsonResp;

}

bool Connection::isConnected()
{
    return _socket->state() == QAbstractSocket::ConnectedState;
}

void Connection::write(const QJsonObject& object)
{
    QByteArray rawData = QJsonDocument(object).toBinaryData();

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint32)0;
    out << rawData;
    out.device()->seek(0);
    out << (quint32)(packet.size() - sizeof(quint32));

    if (_socket->isWritable()) {
        _socket->write(packet);
        _socket->waitForBytesWritten(5000);
        _socket->flush();
    }
}

QJsonObject Connection::read()
{
    QByteArray response;
    quint32 packetSize = 0;

    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_5_0);


    while (_socket->bytesAvailable() < sizeof(quint32)) {
        if (!_socket->waitForReadyRead()) {
            // тут нужно кинуть нормальный exception, что из сокета не удалось прочитать
            throw std::bad_alloc();
        }
    }

    in >> packetSize;

    QByteArray packet;

    while (_socket->bytesAvailable() < packetSize) {
        if (!_socket->waitForReadyRead()) {
            // тут нужно кинуть нормальный exception, что из сокета не удалось прочитать
            qDebug() << "Socket error: " << _socket->errorString();
            throw std::bad_cast();
        }
    }

    packet.append(_socket->read(packetSize));

    QDataStream packetStream(&packet, QIODevice::ReadOnly);
    packetStream >> response;

    return QJsonDocument::fromBinaryData(response).object();
}


Connection::~Connection() {
    _socket->close();
    delete _socket;
}
