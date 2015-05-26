#ifndef ROUTER_H
#define ROUTER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include <QMutex>
#include "Cache/cache.h"
#include "NetworkManager/networkmanager.h"
#include "hashring.h"

class Router : public QTcpServer {
  Q_OBJECT
 public:
  Router(QObject *parent = 0);
  Router(Cache<QString, QString> *rbtree, NetworkManager *manger,
                  QObject *parent = 0);
  void startServer();

 protected:
  void incomingConnection(qintptr handle);

 private:
  QMutex* _mutex;
  QThreadPool *pool;
  Cache<QString, QString> *rbtree;
  NetworkManager *_manager;
  HashRing *_ring;
};

#endif
