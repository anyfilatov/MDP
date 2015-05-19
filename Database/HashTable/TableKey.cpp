#ifndef TABLEKEY_CPP
#define TABLEKEY_CPP

#include "AbstractTableKey.h"
#include "TableKey.h"
#include <QString>
#include <iostream>
#include <sstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

using namespace std;

TableKey::TableKey(){
    userId = -1;
    dataId = -1;
    processId = 0;
}

TableKey::TableKey(short int userId, short int dataId){
    this->userId = userId;
    this->dataId = dataId;
    processId = 0;
}

TableKey::TableKey(short int userId, short int dataId, short int processId){
    this->userId = userId;
    this->dataId = dataId;
    this->processId = processId;
}

TableKey::~TableKey(){
}

short TableKey::getUserId() const{
    return userId;
}

void TableKey::setUserId(short userId){
    this->userId = userId;
}

short TableKey::getDataId() const{
    return dataId;
}

void TableKey::setDataId(short dataId){
    this->dataId = dataId;
}

short TableKey::getProcessId() const{
    return processId;
}

void TableKey::setProcessId(short processId){
    this->processId = processId;
}

void TableKey::setKey(short userId, short dataId, short processId){
    this->userId = userId;
    this->dataId = dataId;
    this->processId = processId;
}

int TableKey::hash(){
    return (processId + dataId + userId);
}

Clonable* TableKey::clone(){
    return new TableKey(userId, dataId, processId);
}

bool TableKey::equals(AbstractTableKey* other){
    TableKey* otherKey = (TableKey*) other;
    return ((userId == otherKey->getUserId()) && (dataId == otherKey->getDataId()) && (processId == otherKey->getProcessId()));
}

QString TableKey::serialize(){
    QJsonObject jsonObj;
    jsonObj.insert("userId", userId);
    jsonObj.insert("dataId", dataId);
    jsonObj.insert("processId", processId);
    QJsonDocument jdoc(jsonObj);
    QString str(jdoc.toJson());
    return str;
}

void TableKey::parse(QString json){
     QJsonDocument jdoc;
     jdoc = jdoc.fromJson(json.toUtf8());
     QJsonObject jsonObj;
     jsonObj = jdoc.object();

     QJsonValue val = jsonObj.take("userId");
     userId = val.toInt();
     val = jsonObj.take("dataId");
     dataId = val.toInt();
     val = jsonObj.take("processId");
     processId = val.toInt();
}



#endif
