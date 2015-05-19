#include "connect.h"
#include "router.h"
#include "typerequest.h"
#include "RouterClient/routerclient.h"
#include "Router/StatusCodes.h"
#include "Exception/serverunavailableexception.h"

Connect::Connect(qintptr handle, HashRing* ring, Cache<QString, QString>* map, QObject* parent)
    : QObject(parent) {
  _socketDescriptor = handle;
  _ring = ring;
  _rbtree = map;
}

void Connect::run() {
  _socket = new QTcpSocket();
  if (!_socket->setSocketDescriptor(_socketDescriptor))
      return;

  QJsonObject jsonReq = read();
  if (jsonReq.value("type") == PUT){
      handleRequest(jsonReq);
  }
  else{
    QJsonObject jsonResp = handleRequest(jsonReq);
    write(jsonResp);
  }
}

QJsonObject Connect::deserialize(QByteArray data) {
  QJsonDocument jdoc = QJsonDocument::fromBinaryData(data);
  return jdoc.object();
}

QByteArray Connect::serialize(QJsonObject json) {
  QJsonDocument jdoc(json);
  return jdoc.toBinaryData();
}

QJsonObject Connect::handleRequest(QJsonObject json) {
  switch (json.find("type").value().toInt()) {
    case PUT:
      return handlePut(json);
    case PUT_OVERRIDE:
      return handlePut(json);
    case REPLACE:
      return handleReplace(json);
    case GET:
      return handleGet(json);
    case DEL:
      return handleRemove(json);
    case DEL_ONE:
        return handleRemove(json);
    case RINGCECK:
      return handleRingCheck(json);
    case OUTERJOIN:
      return handleRingJoin(json);
    default:
      break;
  }
}

QJsonObject Connect::handlePut(QJsonObject json) {
  qDebug() << "do PUT: " << json;
  QJsonObject jsonResp;
  if (json.contains("not_forwards_requests") &&
      json.value("not_forwards_requests").toBool()) {

      QJsonArray value = json.value("packet").toArray();
      for (QJsonValue obj : value){
          QJsonObject elem = obj.toObject();
          _rbtree->insert(elem.value("key").toString(), elem.value("value").toString());
      }

    jsonResp.insert("status", StatusCode::OK);
  } else {
    QList<Node*> nodes;
    nodes = _ring->findNodes(json.value("key").toString());
    qDebug() << "Found nodes:";
    for (Node* node : nodes) {
        qDebug() << node->getAddress();
    }
    RouterClient clientPrimary, clientReplica;
    QJsonObject jsonPrimaryResp = clientPrimary.doRequestToOtherRouter(
      json, nodes[0]->getHost(), nodes[0]->getPort());
    QJsonObject jsonReplicaResp = clientReplica.doRequestToOtherRouter(
      json, nodes[1]->getHost(), nodes[1]->getPort(), true);
    StatusCode codePrimary =
      (StatusCode)jsonPrimaryResp.value("status").toInt();
    StatusCode codeReplica =
      (StatusCode)jsonReplicaResp.value("status").toInt();
    if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
      jsonResp.insert("status", StatusCode::OK);
    } else {
      jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
    }
    for (Node* node : nodes) delete node;
  }
  return jsonResp;
}

QJsonObject Connect::handleReplace(QJsonObject json) {
  QJsonObject jsonResp;
  if (json.contains("not_forwards_requests") &&
      json.value("not_forwards_requests").toBool()) {
    _rbtree->replace(json.value("key").toString(),
                     json.value("values").toVariant().toStringList());
    jsonResp.insert("status", StatusCode::OK);
  } else {
    QList<Node*> nodes;
    nodes = _ring->findNodes(json.value("key").toString());

    RouterClient client;
    QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(
        json, nodes[0]->getHost(), nodes[0]->getPort());
    QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(
        json, nodes[1]->getHost(), nodes[1]->getPort(), true);
    StatusCode codePrimary =
        (StatusCode)jsonPrimaryResp.value("status").toInt();
    StatusCode codeReplica =
        (StatusCode)jsonReplicaResp.value("status").toInt();
    if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
      jsonResp.insert("status", StatusCode::OK);
    } else {
      jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
    }
    for (Node* node : nodes) delete node;
  }

  return jsonResp;
}

QJsonObject Connect::handleGet(QJsonObject json) {
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
        json, nodes[1]->getHost(), nodes[1]->getPort(), true);
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

QJsonObject Connect::handleRemove(QJsonObject json) {
  QJsonObject jsonResp;
  if (json.contains("not_forwards_requests") &&
      json.value("not_forwards_requests").toBool()) {
      StatusCode delCode;
      if (json.find("type").value().toInt() == DEL_ONE) {
        delCode = (StatusCode)_rbtree->remove(json.value("key").toString(), json.value("value").toString());
      } else {
        delCode = (StatusCode)_rbtree->remove(json.value("key").toString());
      }
    jsonResp.insert("status", delCode);
  } else {
    QList<Node*> nodes;
    nodes = _ring->findNodes(json.value("key").toString());
    RouterClient client;
    QJsonObject jsonPrimaryResp = client.doRequestToOtherRouter(
        json, nodes[0]->getHost(), nodes[0]->getPort());
    QJsonObject jsonReplicaResp = client.doRequestToOtherRouter(
        json, nodes[1]->getHost(), nodes[1]->getPort(), true);
    StatusCode codePrimary =
        (StatusCode)jsonPrimaryResp.value("status").toInt();
    StatusCode codeReplica =
        (StatusCode)jsonReplicaResp.value("status").toInt();

    if (codePrimary == StatusCode::OK || codeReplica == StatusCode::OK) {
      jsonResp.insert("status", StatusCode::OK);
    } else {
      jsonResp.insert("status", StatusCode::SERVER_UNAVAILABLE);
    }
    for (Node* node : nodes) delete node;
  }
  return jsonResp;
}

QJsonObject Connect::handleRingCheck(QJsonObject json) {
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

QJsonObject Connect::handleRingJoin(QJsonObject json) {
  QJsonObject jsonResp;
  QStringList values = json.value("ring").toVariant().toStringList();
  QList<Node*> members = _ring->getAllMember();
  QList<Node*> nodes;
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

void Connect::write(const QJsonObject &packet)
{
    QByteArray rawData = QJsonDocument(packet).toBinaryData();

    QByteArray block;
    QDataStream in(&block, QIODevice::WriteOnly);
    in.setVersion(QDataStream::Qt_5_0);
    in << (quint32)rawData.size();
    in.writeRawData(rawData.constData(), rawData.size());

    _socket->write(block);
    _socket->flush();
    _socket->waitForBytesWritten();
}

QJsonObject Connect::read()
{
    QByteArray response;

    quint32 packetSize = 0;
    bool packetRecieved = false;

    QDataStream out(_socket);
    out.setVersion(QDataStream::Qt_5_0);

    if (_socket->waitForReadyRead(5000)) {
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


Connect::~Connect() {
  _socket->close();
  delete _socket;
}
