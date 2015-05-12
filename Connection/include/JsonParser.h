#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include "host.h"
#include <vector>
#include "errors.h"

class JsonParser {
public:
    JsonParser(const QString& fileName) : fileName_(fileName) {
        readFile();
    };
    JsonParser(const JsonParser& orig) = delete;
    virtual ~JsonParser(){};
    
    std::vector<Host> getHosts() {
        std::vector<Host> out;
        QJsonDocument d = QJsonDocument::fromJson(jsonStr_.toUtf8());
        QJsonObject sett2 = d.object();
        auto hosts = sett2.value("hosts").toArray();
        LOG_DEBUG("hosts.size = " << hosts.size());
        for( int i = 0; i < hosts.size(); i++ ) {
            auto jHost = hosts[i].toObject();
            auto ip = jHost["ip"].toString();
            if(ip.isEmpty()) {
                throw ParseConfigException("empty ip");
            }
            int port = jHost["port"].toInt();
            if(port <=0 ) {
                throw ParseConfigException("port <= 0");
            }
            int id = jHost["id"].toInt();
            if(id <=0 ) {
                throw ParseConfigException("id <= 0");
            }
            Host host(ip, port);
            host.setId(id);
            out.push_back(host);
        }
        return out;
    }
    
private:
    void readFile() {
        QFile file;
        file.setFileName(fileName_);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        jsonStr_ = file.readAll();
        file.close();
    }
private:
    QString jsonStr_;
    QString fileName_;
    
};
