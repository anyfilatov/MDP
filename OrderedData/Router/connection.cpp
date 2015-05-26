#include "connection.h"
#include "router.h"
#include "ClientLib/typerequest.h"
#include "ClientLib/StatusCodes.h"
#include "Exception/serverunavailableexception.h"
#include "Cache/rbtree.h"
#include "RouterClient/routerclient.h"

Connection::Connection(qintptr handle, HashRing* ring, Cache<QString, QString>* map, QMutex *mutex, QObject* parent)
    : QObject(parent) {
  _socketDescriptor = handle;
  _ring = ring;
  _rbtree = map;
  _mutex = mutex;
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
                QJsonObject reponse = handleRequest(request);
                write(reponse);
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
    }
}

void Connection::handleBatchRequest(const QJsonObject &json)
{
    if (json.contains("not_forwards_request") && json["not_forwards_request"].toBool()) {
        for (QJsonValue value: json["packet"].toArray()) {
            QJsonObject object = value.toObject();
            switch (object["type"].toInt()) {
            case PUT: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                bool isReplica = object.value("isReplica").toBool();
                _rbtree->insert(key, value, false, isReplica);
                break;
            }
            case PUT_REWRITE: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                bool isReplica = object.value("isReplica").toBool();
                _rbtree->insert(key, value, true, isReplica);
                break;
            }
            case DEL: {
                QString key = object.value("key").toString();
                _rbtree->remove(key);
                break;
            }
            case DEL_ONE: {
                QString key = object.value("key").toString();
                QString value = object.value("value").toString();
                _rbtree->remove(key, value);
                break;
            }
            default:
                break;
            }
        }
    } else {
        QList<Node*> nodes;
        RBTree<QString, QJsonObject> temp;

        for (QJsonValue kv : json.value("packet").toArray()) {
            nodes = _ring->findNodes(kv.toObject().value("key").toString());

            QJsonObject primary(kv.toObject());
            primary.insert("isReplica", false);

            QJsonObject replica(kv.toObject());
            replica.insert("isReplica", true);

            temp.insert(nodes[0]->getAddress(), primary);
            temp.insert(nodes[1]->getAddress(), replica);
        }

        for (QString node : temp.getKeys()) {
            RouterClient client;
            QJsonObject proscessedJson;
            proscessedJson.insert("type", BATCH);
            QJsonArray values;
            for (QJsonObject obj : temp.search(node)) {
                values.append(obj);
            }
            proscessedJson.insert("packet", values);
            qDebug() << "Sending packet to node: " << node;
            client.doRequestToOtherRouter(
                        proscessedJson, node.split(":").first(), node.split(":").last().toInt());
        }
        for (Node* node : nodes) delete node;
    }
}

QJsonObject Connection::handleGet(const QJsonObject &json)
{
    QJsonObject jsonResp;
    if (json.contains("not_forwards_requests") &&
        json.value("not_forwards_requests").toBool()) {
      QStringList values = _rbtree->search(json.value("key").toString());
      jsonResp.insert("key", json.value("key"));
      jsonResp.insert("values", QJsonValue(QJsonArray::fromStringList(values)));
      jsonResp.insert("status", StatusCode::OK);
    } else {
      QList<Node*> nodes;
      nodes = _ring->findNodes(json.value("key").toString());

      RouterClient client;
      QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(
          json, nodes[0]->getHost(), nodes[0]->getPort());
      QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(
          json, nodes[1]->getHost(), nodes[1]->getPort());
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
      for (Node* node : nodes) delete node;
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
        if (!_socket->waitForReadyRead(5000)) {
            // тут нужно кинуть нормальный exception, что из сокета не удалось прочитать
            throw ServerUnavailableException();
        }
    }

    in >> packetSize;

    QByteArray packet;

    while (_socket->bytesAvailable() < packetSize) {
        if (!_socket->waitForReadyRead(5000)) {
            // тут нужно кинуть нормальный exception, что из сокета не удалось прочитать
            throw ServerUnavailableException();
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
