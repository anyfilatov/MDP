#include "router.h"
#include "connection.h"

Router::Router(QObject *parent) : QTcpServer(parent) {
  pool = new QThreadPool(this);
  pool->setMaxThreadCount(1024);
}

Router::Router(Cache<QString, QString> *rbtree, NetworkManager *manager,
               QObject *parent)
    : QTcpServer(parent) {
  pool = new QThreadPool(this);
  pool->setMaxThreadCount(1024);
  this->rbtree = rbtree;
  _manager = manager;
  _ring = new HashRing(_manager, this->rbtree);
  _mutex = new QMutex();
}

void Router::startServer() {
  if (this->listen(QHostAddress(_manager->getSettings()->getHost()),
                   _manager->getSettings()->getPort() + 1)) {
    qDebug() << "Server started";
  } else {
    qDebug() << "Server did not start!";
  }
}

void Router::incomingConnection(qintptr handle) {
  // 1. QTcpServer gets a new connection request from a client.
  // 2. It makes a task (runnable) here.
  // 3. Then, the server grabs one of the threads.
  // 4. The server throws the runnable to the thread.

  // Note: Rannable is a task not a thread
  Connection *task = new Connection(handle, _ring, this->rbtree, _mutex);

  task->setAutoDelete(true);

  pool->start(task);

//  qDebug() << "pool started";
}
