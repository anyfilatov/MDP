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
    Server(QString ip, int port);
    Server(const Server& orig) = delete;
    virtual ~Server();
    int Run();
    virtual int send(const HostContent& cmd)const;
private:
    int configure();
    int unpack(QTcpSocket& socket, QString* data);
    void sendOk(QTcpSocket& socket);
    void sendError(QTcpSocket& socket);
private:
    QTcpServer tcpServer_;
    mutable ThreadPool<Task> threadPool_;
    OrGraph<Host> orGraph_;
    Wrapper<TaskCounterMap> tasksMap_;
    Wrapper<RbTree> rbTree_;
    Wrapper<DataBase> dataBase_;
};


