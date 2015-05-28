#ifndef CACHEINFOSERVICE_H
#define CACHEINFOSERVICE_H

#include <QVector>
#include "CacheInfo.h"
#include "ClientLib/guiclient.h"

class CacheInfoService
{
public:
    CacheInfoService();
    ~CacheInfoService();

    void updateData();

    QList<CacheInfo> nodes() const { return nodes_; }

private:
    QList<CacheInfo> nodes_;
    GUIClient *client;

    QString settingsFromFile();
    QString stringFromFile(QString name);
    void parseJSON(const QJsonArray& array);
    void testData();

};

#endif // CACHEINFOSERVICE_H
