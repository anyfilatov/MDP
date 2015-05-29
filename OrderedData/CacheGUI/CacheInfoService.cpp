#include "CacheInfoService.h"
#include <QMessageBox>

//#define DEBUG

CacheInfoService::CacheInfoService()
{
    testData();
    client = new GUIClient("settings.json");
}

QString CacheInfoService::settingsFromFile()
{
    return stringFromFile("../settings.json");
}

QString CacheInfoService::stringFromFile(QString name)
{
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QString string;
    string = file.readAll();
    file.close();

    return string;
}

CacheInfoService::~CacheInfoService()
{
//    delete client;
}

void CacheInfoService::updateData()
{
    nodes_.clear();
#ifdef DEBUG
    QString json = stringFromFile("://testgui.json");
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
//    qDebug() << doc.toJson() << doc.isEmpty();
    parseJSON(doc.array());
#else
    parseJSON(client->getJSONData());
#endif
}

void CacheInfoService::parseJSON(const QJsonArray& array)
{
    foreach (QJsonValue value, array) {
        qDebug() << "json";
        QJsonObject obj = value.toObject();
        CacheInfo info;
        info.setIp(obj["host"].toString());
//        info.setWorkTime(obj[""].toString());
//        info.setRequestsPerMinute(obj[""].toString());
        info.setKeysCount(QString::number(obj["keys_count"].toInt()));

        QString primary = QString::number(obj["primary_count"].toInt());
        QString replica = QString::number(obj["replica_count"].toInt());
        info.setKeysPerReplics(primary + "/" + replica);

        info.setHits(QString::number(obj["hits_count"].toDouble()));

        info.setTreeHeight(QString::number(obj["height_count"].toInt()));

        int red = obj["red_nodes_count"].toInt();
        int black = obj["black_nodes_count"].toInt();
        double result = ((double)black / (red + black)) * 100;
        info.setBlackNodesPercent((int)result);
        qDebug() << "rb" << info.blackNodesPercent() << result << black << red;
        nodes_.append(info);
    }
}

void CacheInfoService::testData()
{
    CacheInfo info1 = CacheInfo("192.168.82.9", "100", "100", "100", "100", "100", "35", 45);
    CacheInfo info2 = CacheInfo("192.168.105.2", "100", "100", "100", "100", "100", "35", 55);
    CacheInfo info3 = CacheInfo("192.168.101.5", "100", "100", "100", "100", "100", "35", 65);
    nodes_.append(info1);
    nodes_.append(info2);
    nodes_.append(info3);
}
