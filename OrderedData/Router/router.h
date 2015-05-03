#ifndef ROUTER_H
#define ROUTER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include "../Cache/icache.h"

class Router : public QTcpServer
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = 0);
    explicit Router(iCache<QString, QString> *rbtree, QObject *parent = 0);
    void startServer();

protected:
    void incomingConnection( qintptr handle );
    void test();

private:
    QThreadPool *pool;
    iCache<QString, QString> *rbtree;
};

#endif
