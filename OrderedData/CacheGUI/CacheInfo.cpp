#include "CacheInfo.h"

CacheInfo::CacheInfo()
{
    ip_ = "";
    workTime_ = "";
    keysCount_ = "";
    requestsPerMinute_ = "";
    keysPerReplics_ = "";
    hits_ = "";
    treeHeight_ = "";
    blackNodesPercent_ = 50;
}

CacheInfo::CacheInfo(QString ip, QString workTime, QString keysCount,
                     QString requestPerMinute, QString keysPerMinute,
                     QString hits, QString treeHeight, int blackNodesPercent)
{
    ip_ = ip;
    workTime_ = workTime;
    keysCount_ = keysCount;
    requestsPerMinute_ = requestPerMinute;
    keysPerReplics_ = keysPerMinute;
    hits_ = hits;
    treeHeight_ = treeHeight;
    blackNodesPercent_ = blackNodesPercent;
}

CacheInfo::~CacheInfo()
{

}

