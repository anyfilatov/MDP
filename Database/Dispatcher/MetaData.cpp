#ifndef METADATA_CPP
#define METADATA_CPP

#include <QString>
#include "Serializible.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <QTextStream>
#include "Dispatcher/MetaData.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

using namespace std;

namespace database{

MetaData::MetaData(){
    size = 0;
}

MetaData::MetaData(std::vector<QString> headers, int size){
    this->size = size;
    this->headers.swap(headers);
}

MetaData::~MetaData(){
    headers.clear();
    size = 0;
}

void MetaData::setSize(int size){
    this->size = size;
}

int MetaData::getSize(){
    return size;
}

void MetaData::setHeaders(vector<QString>& headers){
    this->headers.swap(headers);
}

vector<QString>& MetaData::getHeaders(){
    return headers;
}

QString MetaData::serialize(){
    QJsonObject mainObj, headersObj;
    headersObj.insert("size", (int)headers.size());
    for (int i = 0; i < headers.size(); i++){
        headersObj.insert(QString::number(i), headers[i]);
    }
    mainObj.insert("headers", headersObj);
    mainObj.insert("size", size);
    QJsonDocument mainDoc(mainObj);
    QString json(mainDoc.toJson());
    return json;
}

void MetaData::parse(QString json){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(json.toUtf8());
    QJsonObject jsonObj;
    jsonObj = jdoc.object();

    QJsonValue val = jsonObj.take("size");
    this->size = val.toInt();

    val = jsonObj.take("headers");
    QJsonObject headersObj = val.toObject();
    val = headersObj.take("size");
    int headersSize = val.toInt();
    headers.resize(headersSize);
    for (int i = 0; i < headersSize; i++){
        val = headersObj.take(QString::number(i));
        headers[i] = val.toString();
    }
}

}

#endif
