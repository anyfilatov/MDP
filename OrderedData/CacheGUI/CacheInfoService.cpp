#include "CacheInfoService.h"

CacheInfoService::CacheInfoService()
{
    testData();
}

CacheInfoService::~CacheInfoService()
{

}

void CacheInfoService::testData()
{
    CacheInfo info1 = CacheInfo("ip", "100", "100", "100", "100", "100", "35", 45);
    CacheInfo info2 = CacheInfo("192.168.105.2", "100", "100", "100", "100", "100", "35", 55);
    CacheInfo info3 = CacheInfo("192.168.101.5", "100", "100", "100", "100", "100", "35", 65);
    nodes_.append(info1);
    nodes_.append(info2);
    nodes_.append(info3);
}

