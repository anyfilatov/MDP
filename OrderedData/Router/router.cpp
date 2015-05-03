#include "router.h"
#include "connect.h"

Router::Router(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
    map = new QMap<QString, QString>;
}

void Router::startServer()
{
    if(this->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server started";
    }
    else
    {
        qDebug() << "Server did not start!";
    }
}

void Router::incomingConnection(qintptr handle)
{
    // 1. QTcpServer gets a new connection request from a client.
    // 2. It makes a task (runnable) here.
    // 3. Then, the server grabs one of the threads.
    // 4. The server throws the runnable to the thread.

    // Note: Rannable is a task not a thread
    Connect *task = new Connect(this->map);

    task->setAutoDelete(true);
    task->socketDescriptor = handle;

    pool->start(task);
    qDebug() << map->values();

    qDebug() << "pool started";
}
