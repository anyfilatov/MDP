#include "hashring.h"
#include <QtAlgorithms>
#include <QTimer>

#include "RouterClient/routerclient.h"
#include "Cache/qrbnode.h"

HashRing::HashRing() {}

HashRing::HashRing(NetworkManager* manager, Cache<QString, QString>* cache,
                   QObject* parent)
    : QObject(parent) {
  _manager = manager;
  _cache = cache;

  // Building ring and initialize owners of replicas
  for (Node* node : manager->getAllMembers()) {
    _ring.push_back(node);
  }


  for(Node* node: _ring) {
      qDebug() << node->getAddress();
  }
  qSort(_ring.begin(), _ring.end(), hashBasedLessThen);
  qDebug() << _manager->getMyself();


  if (_ring.indexOf(_manager->getMyself()) == 0) {
    _haveReplicaOf = _ring[_ring.size() - 1];
  } else {
    _haveReplicaOf = _ring[_ring.indexOf(_manager->getMyself()) - 1];
  }

  _hasMyReplica =
      _ring[(_ring.indexOf(_manager->getMyself()) + 1) % _ring.size()];
  // =============================================

  QTimer* timer = new QTimer();
  timer->setInterval(_manager->getSettings()->getCleanupInterval() * 2);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start();
}

HashRing::~HashRing() {}

QList<Node> HashRing::findNodes(QString key) {
  unsigned int pos = hash(key);
  QList<Node> nodes;

  Node* firstNode;
  Node* secondNode;

  if (_ring.size() >= 2) {
    if (pos <= hash(_ring.at(0)->getAddress()) ||
        pos > hash(_ring.at(_ring.size() - 1)->getAddress())) {
        firstNode = _ring.at(0);
        secondNode = _ring.at(1);
      nodes.push_back(Node(firstNode->getHost(), firstNode->getPort()+1));
      nodes.push_back(Node(secondNode->getHost(), secondNode->getPort()+1));
    } else {
      for (int i = 0; i < _ring.size(); ++i) {
        if (pos <= hash(_ring.at(i)->getAddress())) {
            firstNode = _ring.at(i);
            secondNode = _ring.at((i + 1) % _ring.size());
          nodes.push_back(Node(firstNode->getHost(), firstNode->getPort() + 1));
          nodes.push_back(Node(secondNode->getHost(), secondNode->getPort() + 1));
          break;
        }
      }
    }
  }
  return nodes;
}

void HashRing::update() {
  qDebug() << "update ring";
  qDebug() << "RBTree nodes size: " << _cache->getRBTree()->size();
  qDebug() << _ring;
  QList<Node*> curMemList;
  bool change = false;

  for (Node* node : _manager->getAllMembers()) {
    curMemList.push_back(node);
  }

  qSort(curMemList.begin(), curMemList.end(), hashBasedLessThen);

  qDebug() << "Current member list: " << curMemList;
  if (_ring.size() != curMemList.size() && !_cache->isEmpty() ) {
    change = true;
  }

  if (change) {
    qDebug() << "Stabilization is required";
    _ring = curMemList;
    stabilize();
  }
}

NetworkManager *HashRing::getManager() const
{
    return _manager;
}


void HashRing::stabilize() {
  qDebug() << "Stabilization started";
  qDebug() << _ring;

  Node* to_be_predecessor;  // new nodes whose replicas I have
  Node* to_be_successor;    // new nodes where my replicas are stored

  if (_ring.indexOf(_manager->getMyself()) == 0) {
    to_be_predecessor = _ring[_ring.size() - 1];
  } else {
    to_be_predecessor = _ring[_ring.indexOf(_manager->getMyself()) - 1];
  }

  to_be_successor =
      _ring[(_ring.indexOf(_manager->getMyself()) + 1) % _ring.size()];

//  qDebug() << _hasMyReplica->getAddress();
//  qDebug() << _haveReplicaOf->getAddress();

  if (_hasMyReplica->getAddress() != to_be_successor->getAddress()) {
    // TODO get all primary keys and send them (i.e. update)  to to_be_successor
    qDebug() << "UPDATE REPLICA KEYS. From "
             << _manager->getMyself()->getAddress() << " to "
             << to_be_successor->getAddress();
    RouterClient client;
//    for (QString key : _cache->getRBTree()->getKeys()) {
//      QJsonObject jsonReq;
//      jsonReq.insert("type", REPLACE);
//      jsonReq.insert("key", key);

//      QRBNode<QString> node = _cache->getRBTree()->getNode(key);
//      if (!node.is_replica()) {
//          QList<QString> values = QVector<QString>::fromStdVector(node.values()).toList();
//          jsonReq.insert("values", QJsonValue(QJsonArray::fromStringList(values)));

//          client.doRequestToOtherRouter(jsonReq, to_be_successor->getHost(),
//                                        to_be_successor->getPort() + 1, true);
//      }
//    }
  }

  if (_haveReplicaOf->getAddress() != to_be_predecessor->getAddress()) {
    // TODO get all replica keys and send them (i.e. update)  to
    // to_be_predecessor
    qDebug() << "UPDATE PRIMARIES KEYS. From "
             << _manager->getMyself()->getAddress() << " to "
             << to_be_predecessor->getAddress();
    RouterClient client;
//    for (QString key : _cache->getRBTree()->getKeys()) {
//      QJsonObject jsonReq;
//      jsonReq.insert("type", REPLACE);
//      jsonReq.insert("key", key);

//      QRBNode<QString> node = _cache->getRBTree()->getNode(key);
//      if (node.is_replica()) {
//          QList<QString> values = QVector<QString>::fromStdVector(node.values()).toList();
//          jsonReq.insert("values", QJsonValue(QJsonArray::fromStringList(values)));
//          qDebug() << "Request" << jsonReq;
//          client.doRequestToOtherRouter(jsonReq, to_be_predecessor->getHost(),
//                                        to_be_predecessor->getPort() + 1);
//      }
//    }
  }

  _hasMyReplica = to_be_successor;
  _haveReplicaOf = to_be_predecessor;
}

unsigned int HashRing::hash(QString key) {
  const char* str = key.toStdString().c_str();
  const size_t length = strlen(str) + 1;
  unsigned int hash = OFFSET_BASIS;
  for (size_t i = 0; i < length; ++i) {
    hash ^= *str++;
    hash *= FNV_PRIME;
  }
  return hash;
}


bool HashRing::hashBasedLessThen(const Node* node1, const Node* node2) {
  return hash(node1->getAddress()) < hash(node2->getAddress());
}

QList<Node*> HashRing::getAllMember() {
    QList<Node*> members =  _manager->getAllMembers();
    QList<Node*> ring_nodes;
    for(Node* member: members) {
        ring_nodes.append(new Node(member->getHost(), member->getPort() + 1));
    }
    return ring_nodes;
}
