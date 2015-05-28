#ifndef CACHEINFO_H
#define CACHEINFO_H

#include <QString>

class CacheInfo
{
public:
    CacheInfo();
    CacheInfo(QString ip, QString workTime, QString keysCount,
              QString requestPerMinute, QString keysPerMinute,
              QString hits, QString treeHeight, int blackNodesPercent);
    ~CacheInfo();

    QString ip() const { return ip_; }
    QString workTime() const { return workTime_; }
    QString keysCount() const { return keysCount_; }
    QString requestsPerMinute() const { return requestsPerMinute_; }
    QString keysPerReplics() const { return keysPerReplics_; }
    QString hits() const { return hits_; }
    QString treeHeight() const { return treeHeight_; }
    int blackNodesPercent() const { return blackNodesPercent_; }

    void setIp(QString ip) { this->ip_ = ip; }
    void setWorkTime(QString workTime) { this->workTime_ = workTime; }
    void setKeysCount(QString keysCount) { this->keysCount_ = keysCount; }
    void setRequestsPerMinute(QString requestsPerMinute) { this->requestsPerMinute_ = requestsPerMinute; }
    void setKeysPerReplics(QString keysPerReplics) { this->keysPerReplics_ = keysPerReplics; }
    void setHits(QString hits) { this->hits_ = hits; }
    void setTreeHeight(QString treeHeight) { this->treeHeight_ = treeHeight; }
    void setBlackNodesPercent(int percent) { this->blackNodesPercent_ = percent; }

private:
    QString ip_;
    QString workTime_;
    QString keysCount_;
    QString requestsPerMinute_;
    QString keysPerReplics_;
    QString hits_;
    QString treeHeight_;
    int blackNodesPercent_;
};

#endif // CACHEINFO_H
