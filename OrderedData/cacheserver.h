#ifndef CACHESERVER_H
#define CACHESERVER_H
#include "Router/router.h"
#include "Cache/cache.h"
#include "NetworkManager/networkmanager.h"

class CacheServer
{
public:
    CacheServer(NetworkSettings& settings);
private:
    Router *router;
    Cache<QString, QString> *cache;
    NetworkManager *manager;

};

#endif // CACHESERVER_H
