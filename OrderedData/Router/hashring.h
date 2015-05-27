#pragma once
#ifndef HASHRING_H
#define HASHRING_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "NetworkManager/networkmanager.h"
#include "Cache/cache.h"

class HashRing : public QObject {
  Q_OBJECT
 public:
  HashRing();
  explicit HashRing(NetworkManager* manager, Cache<QString, QString>* cache,
                    QObject* parent = 0);
  ~HashRing();
  QList<Node> findNodes(QString key);
  QList<Node*> getAllMember();

  NetworkManager *getManager() const;

public slots:
  void update();

private:
  QList<Node*> _ring;
  Node* _hasMyReplica;
  Node* _haveReplicaOf;

  NetworkManager* _manager;
  Cache<QString, QString>* _cache;

  void stabilize();
  static bool hashBasedLessThen(const Node* node1, const Node* node2);

  static unsigned int hash(QString key);
  static const unsigned int FNV_PRIME = 16777619u;
  static const unsigned int OFFSET_BASIS = 2166136261u;

};

#endif  // HASHRING_H
