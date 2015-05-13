#ifndef HASHRING_H
#define HASHRING_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "../NetworkManager/networkmanager.h"
#include "../Cache/icache.h"

class HashRing : public QObject {
  Q_OBJECT
 public:
  HashRing();
  explicit HashRing(NetworkManager* manager, iCache<QString, QString>* cache,
                    QObject* parent = 0);
  ~HashRing();
  QList<Node*> findNodes(QString key);
  QList<Node*> getAllMember();

  NetworkManager *getManager() const;

public slots:
  void update();

private:
  QList<Node*> _ring;
  Node* _hasMyReplica;
  Node* _haveReplicaOf;

  NetworkManager* _manager;
  iCache<QString, QString>* _cache;

  void stabilize();
  static size_t hash(QString key);
  static bool hashBasedLessThen(const Node* node1, const Node* node2);
};

#endif  // HASHRING_H
