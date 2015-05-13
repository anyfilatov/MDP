#ifndef DATA_CPP
#define DATA_CPP

#include <QString>
#include "Serializible.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <QTextStream>
#include "Data.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

using namespace std;

MDPData::MDPData(){
    firstIndex = 0;
}

MDPData::MDPData(vector<QString> headers, vector<vector<QString> > cells, int firstIndex){
    QTextStream cout(stdout);
    //cout << "NEW DATA: headersSize: " << headers.size() << ", cellSize: " << cells.size() << endl;
    this->firstIndex = firstIndex;
    this->headers = headers;
    this->cells = cells;
}

int MDPData::findHeaderColumn(QString header){
    int num = -1;
    for (int i = 0; i < headers.size(); i++){
        if (headers[i] == header){
            num = i;
            break;
        }
    }
    return num;
}

MDPData::~MDPData(){
    headers.clear();
    cells.clear();
}

void MDPData::setFirstIndex(int index){
    this->firstIndex = index;
}

int MDPData::getFirstIndex(){
    return firstIndex;
}

void MDPData::setHeaders(vector<QString> headers){
    this->headers.clear();
    this->headers = headers;
}

vector<QString> MDPData::getHeaders(){
    return headers;
}

void MDPData::setCells(vector<vector<QString> > cells){
    this->cells.clear();
    this->cells = cells;
}

vector<vector<QString> > MDPData::getCells(){
    return cells;
}

vector<QString> MDPData::getString(int num){
    vector<QString> str(0);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size())){
        return str;
    }
    return cells[innerNum];
}

int MDPData::addString(vector<QString> Str){
    if (Str.size() != headers.size()) return -1;
    cells.push_back(Str);
    return (cells.size() + firstIndex - 1);
}

vector<QString> MDPData::getColumn(QString header){
    vector<QString> column(cells.size());
    int num = MDPData::findHeaderColumn(header);
    for (int i = 0; i < cells.size(); i++){
        column[i] = cells[i][num];
    }
    return column;
}

QString MDPData::getCell(QString header, int num){
    int column = MDPData::findHeaderColumn(header);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size()) || (column == -1)) return "";
    return cells[innerNum][column];
}

int MDPData::size(){
    return cells.size();
}

QString MDPData::serialize(){
    QTextStream cout(stdout);

    QJsonObject mainObj, headersObj;
    headersObj.insert("size", (int)headers.size());
    for (int i = 0; i < headers.size(); i++){
        headersObj.insert(QString::number(i), headers[i]);
    }
    /* QJsonDocument headersDoc(headersObj);
    QString headersString(headersDoc.toJson());*/
    mainObj.insert("headers", headersObj);
    mainObj.insert("firstIndex", firstIndex);
    mainObj.insert("size", (int)cells.size());
    for (int i = 0; i < cells.size(); i++){
        QJsonObject stringObj;
        for (int j = 0; j < cells[i].size(); j++){
            stringObj.insert(QString::number(j), cells[i][j]);
        }
        /*QJsonDocument stringDoc(stringObj);
        QString str(stringDoc.toJson());*/
        mainObj.insert(QString::number(i), stringObj);
    }
    QJsonDocument mainDoc(mainObj);
    QString json(mainDoc.toJson());
    //cout << json << endl;
    return json;
}

void MDPData::parse(QString json){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(json.toUtf8());
    QJsonObject jsonObj;
    jsonObj = jdoc.object();

    QJsonValue val = jsonObj.take("size");
    int size = val.toInt();
    cells.clear();
    cells.resize(size);
    val = jsonObj.take("firstIndex");
    firstIndex = val.toInt();
    val = jsonObj.take("headers");
    QJsonObject headersObj = val.toObject();
    val = headersObj.take("size");
    int headersSize = val.toInt();
    headers.clear();
    headers.resize(headersSize);
    for (int i = 0; i < headersSize; i++){
        val = headersObj.take(QString::number(i));
        headers[i] = val.toString();
    }

    for (int j = 0; j < size; j++){
        cells[j].resize(headersSize);
        val = jsonObj.take(QString::number(j));
        QJsonObject cellsObj = val.toObject();
        for (int i = 0; i < headersSize; i++){
            val = cellsObj.take(QString::number(i));
            cells[j][i] = val.toString();
        }
    }
}


#endif
