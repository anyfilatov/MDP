#pragma once

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QByteArray>
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
    
    std::vector<Host> getHosts(const std::string& name) {
        std::vector<Host> out;
        QJsonDocument d = QJsonDocument::fromJson(jsonStr_.toUtf8());
        QJsonObject sett2 = d.object();
        auto hosts = sett2.value(name.c_str()).toArray();
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
    
    std::map<int, std::vector<int>> readRelations(){
        std::map<int, std::vector<int>> out;
        QJsonDocument d = QJsonDocument::fromJson(jsonStr_.toUtf8());
        QJsonObject sett2 = d.object();
        auto relations = sett2.value("relations").toArray();
        LOG_DEBUG("relations.size = " << relations.size());
        for( int i = 0; i < relations.size(); i++ ) {
            auto jHost = relations[i].toObject();
            auto id = jHost["parent"].toInt();
            if(id <= 0) {
                throw ParseConfigException("incorrect id");
            }
            auto childs = jHost["childs"].toArray();
            auto& vec = out[id];
            for(int j = 0; j < childs.size(); j++) {
                vec.push_back(childs[j].toInt());
            }
        }
        return out;
    }
    
    void change(const std::vector<QString>& conf, const std::vector<QString>& newConf){
//        std::vector<Host> out;
//        QJsonDocument d = QJsonDocument::fromJson(jsonStr_.toUtf8());
//        QJsonObject sett2 = d.object();
//        auto hosts = sett2.value("hosts").toArray();
//        LOG_DEBUG("hosts.size = " << hosts.size());
//        for(int j = 0; j < conf.size(); ++j) {
//            for( int i = 0; i < hosts.size(); i++ ) {
//                auto jHost = hosts[i].toObject();
//                auto ip = jHost["ip"].toString();
//                if(ip.isEmpty()) {
//                    throw ParseConfigException("empty ip");
//                }
//                int id = jHost["id"].toInt();
//                if(id <=0 ) {
//                    throw ParseConfigException("id <= 0");
//                }
//                int port = jHost["port"].toString();
//                if( conf[j] == ip +  ":" + port){
//                   QJsonObject 
//                }
//
//            }
//        }
//        return out;
    }
    
private:
    void readFile() {
        QFile file;
        file.setFileName(fileName_);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        jsonStr_ = file.readAll();
        file.close();
    }
    void writeToFie(){
//        QFile file;
//        file.setFileName(fileName_);
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        QByteArray ar(jsonStr_);
//        file.write(ar);
//        file.close();
    }
private:
    QString jsonStr_;
    QString fileName_;
    
};
