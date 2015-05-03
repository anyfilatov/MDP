#ifndef CACHESERVER_H
#define CACHESERVER_H
#include "Router/router.h"
#include "Cache/cache.h"

class CacheServer
{
public:
    CacheServer();
private:
    Router *router;
    Cache<QString, QString> *cache;

};

#endif // CACHESERVER_H
