#pragma once

#include <string>
#include "wrapper.h"
#include "host.h"
#include "DataBase.h"
#include "RbTree.h"
#include "OrGraph.h"
#include "LuaExecutor.h"
#include <QTcpSocket> 

class Task 
{
    RB rb_;
    DB db_;
    HostContent content_;
    OG og_;
    std::shared_ptr<LuaExecutor> executor_;
public:
    Task(const HostContent& content, RB rb, DB db, OG og);
    ~Task(void);
    int operator()();
private:
    int unpackAndExec(int cmd);
    int ping(QDataStream&);
    int setConfig(QDataStream&);
    int doMap(QDataStream&);
    int doReduce(QDataStream&);
    int doUserScript(QDataStream&);
    int doPing(QDataStream&);
        
};


