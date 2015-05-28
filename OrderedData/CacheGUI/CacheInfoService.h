#ifndef CACHEINFOSERVICE_H
#define CACHEINFOSERVICE_H

#include <QVector>
#include "CacheInfo.h"

class CacheInfoService
{
public:
    CacheInfoService();
    ~CacheInfoService();

    QList<CacheInfo> nodes() const { return nodes_; }

private:
    QList<CacheInfo> nodes_;

    void testData();
};

#endif // CACHEINFOSERVICE_H
