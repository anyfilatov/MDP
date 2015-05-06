#include "cacheserver.h"

CacheServer::CacheServer(NetworkSettings& settings)
{
    this->cache = new Cache<QString, QString>;
    this->router = new Router(cache);
    router->startServer();
    this->manager = new NetworkManager(settings);
    QThread *managerThread = new QThread();
    router->moveToThread(managerThread);
    managerThread->start();


}

