#include "cacheserver.h"

CacheServer::CacheServer(NetworkSettings& settings)
{
    this->cache = new Cache<QString, QString>;
    this->manager = new NetworkManager(settings);
    this->router = new Router(cache, manager);
    router->startServer();
    QThread *managerThread = new QThread();
    router->moveToThread(managerThread);
    managerThread->start();


}

