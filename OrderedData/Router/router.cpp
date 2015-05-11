#include "router.h"
#include "connect.h"

Router::Router(QObject *parent) : QTcpServer(parent) {
  pool = new QThreadPool(this);
  pool->setMaxThreadCount(3);
  // rbtree = new RBTree();
}

Router::Router(iCache<QString, QString> *rbtree, NetworkManager *manager,
               QObject *parent)
    : QTcpServer(parent) {
  pool = new QThreadPool(this);
  pool->setMaxThreadCount(3);
  this->rbtree = rbtree;
  this->rbtree->insert("test", "test");
  _manager = manager;
  _ring = new HashRing(_manager, this->rbtree);
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
  Connect *task = new Connect(_ring, this->rbtree);

  task->setAutoDelete(true);
  task->socketDescriptor = handle;

  pool->start(task);

  qDebug() << "pool started";
}
