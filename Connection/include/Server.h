#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include "ThreadPool.h"
#include <memory>
#include <map>
#include <mutex>
#include "Task.h"
#include "orgraph.h"
#include "RbTree.h"
#include "DataBase.h"
#include "host.h"
#include "Content.h"

typedef std::map<int, int> TaskCounterMap;

typedef std::shared_ptr<Task> TaskPtr;

class Server : public Host{
    const int WAIT_CONNECTION_TIME_MSEC = 1000000;
    const int THREAD_COUNT = 2;
public:
    Server(QString ip, int port, const DB& db, const RB& rb, OG& og);
    Server(const Server& orig) = delete;
    virtual ~Server();
    virtual int run();
    virtual int send(const HostContent& cmd)const;
private:
    int configure();
    int unpack(QTcpSocket& socket, QString* data);
    void sendOk(QTcpSocket& socket);
    void sendError(QTcpSocket& socket);
private:
    QTcpServer tcpServer_;
    mutable ThreadPool<Task> threadPool_;
    Wrapper<TaskCounterMap> tasksMap_;
    RB rbTree_;
    DB dataBase_;
    OG orGraph_;
};


