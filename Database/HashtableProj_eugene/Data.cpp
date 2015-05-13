#ifndef DATA_CPP
#define DATA_CPP

#include <QString>
//#include "JsonMethods.h"
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

Data::Data(){
    firstIndex = 0;
}

Data::Data(vector<QString> headers, vector<vector<QString> > cells, int firstIndex){
    QTextStream cout(stdout);
    cout << "NEW DATA: headersSize: " << headers.size() << ", cellSize: " << cells.size() << endl;
    this->firstIndex = firstIndex;
    this->headers = headers;
    this->cells = cells;
}

int Data::findHeaderColumn(QString header){
    int num = -1;
    for (int i = 0; i < headers.size(); i++){
        if (headers[i] == header){
            num = i;
            break;
        }
    }
    return num;
}

Data::~Data(){
    headers.clear();
    cells.clear();
}

void Data::setFirstIndex(int index){
    this->firstIndex = index;
}

int Data::getFirstIndex(){
    return firstIndex;
}

void Data::setHeaders(vector<QString> headers){
    this->headers.clear();
    this->headers = headers;
}

vector<QString> Data::getHeaders(){
    return headers;
}

void Data::setCells(vector<vector<QString> > cells){
    this->cells.clear();
    this->cells = cells;
}

vector<vector<QString> > Data::getCells(){
    return cells;
}

vector<QString> Data::getString(int num){
    vector<QString> str(0);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size())){
        return str;
    }
    return cells[innerNum];
}

int Data::addString(vector<QString> Str){
    if (Str.size() != headers.size()) return -1;
    cells.push_back(Str);
    return (cells.size() + firstIndex - 1);
}

vector<QString> Data::getColumn(QString header){
    vector<QString> column(cells.size());
    int num = Data::findHeaderColumn(header);
    for (int i = 0; i < cells.size(); i++){
        column[i] = cells[i][num];
    }
    return column;
}

QString Data::getCell(QString header, int num){
    int column = Data::findHeaderColumn(header);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size()) || (column == -1)) return "";
    return cells[innerNum][column];
}

int Data::size(){
    return cells.size();
}

QString Data::serialize(){
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
    cout << json << endl;
    return json;
}

void Data::parse(QString json){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(json.toUtf8());
    QJsonObject jsonObj;
    jsonObj = jdoc.object();

    QJsonValue val = jsonObj.take("size");
    int size = val.toInt();
    cells.clear();
    cells = *(new vector<vector<QString> >(size));
    val = jsonObj.take("firstIndex");
    firstIndex = val.toInt();
    val = jsonObj.take("headers");
    QJsonObject headersObj = val.toObject();
    val = headersObj.take("size");
    int headersSize = val.toInt();
    headers.clear();
    headers = *(new vector<QString>(headersSize));
    for (int i = 0; i < headersSize; i++){
        val = headersObj.take(QString::number(i));
        headers[i] = val.toString();
    }

    for (int j = 0; j < size; j++){
        cells[j] = *(new vector<QString>(headersSize));
        val = jsonObj.take(QString::number(j));
        QJsonObject cellsObj = val.toObject();
        for (int i = 0; i < headersSize; i++){
            val = cellsObj.take(QString::number(i));
            cells[j][i] = val.toString();
        }
    }
}


#endif
