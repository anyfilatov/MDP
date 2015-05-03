#include "cacheserver.h"

CacheServer::CacheServer()
{
    this->cache = new Cache<QString, QString>;
    this->router = new Router(cache);
}

