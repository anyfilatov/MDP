#ifndef ROUTER_H
#define ROUTER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>

class Router : public QTcpServer
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = 0);
    void startServer();

protected:
    void incomingConnection( qintptr handle );
    void test();

private:
    QThreadPool *pool;
    QMap<QString, QString> *map;
};

#endif
