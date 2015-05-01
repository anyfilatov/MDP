#ifndef DATA
#define DATA

#include <string>
#include "JsonMethods.h"
#include "Serializible.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Data:public Serializible{
private:
    int firstIndex;
    vector<string> headers;
    vector<vector<string> > cells;
    int findHeaderColumn(string header);
public:
    Data();
    Data(vector<string> headers, vector<vector<string> > cells, int firstIndex);
    ~Data();
    void setFirstIndex(int index);
    int getFirstIndex();
    void setHeaders(vector<string> headers);
    vector<string> getHeaders();
    void setCells(vector<vector<string> > cells);
    vector<vector<string> > getCells();
    vector<string> getString(int num);
    int addString(vector<string> Str);
    vector<string> getColumn(string header);
    string getCell(string header, int num);
    int size();
    string serialize();
    void parse(string json);

};

Data::Data(){
    firstIndex = 0;
}

Data::Data(vector<string> headers, vector<vector<string> > cells, int firstIndex){
    QTextStream cout(stdout);
    cout << "NEW DATA: headersSize: " << headers.size() << ", cellSize: " << cells.size() << endl;
    this->firstIndex = firstIndex;
    this->headers = headers;
    this->cells = cells;
}

int Data::findHeaderColumn(string header){
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

void Data::setHeaders(vector<string> headers){
    this->headers.clear();
    this->headers = headers;
}

vector<string> Data::getHeaders(){
    return headers;
}

void Data::setCells(vector<vector<string> > cells){
    this->cells.clear();
    this->cells = cells;
}

vector<vector<string> > Data::getCells(){
    return cells;
}

vector<string> Data::getString(int num){
    vector<string> str(0);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size())){
        return str;
    }
    return cells[innerNum];
}

int Data::addString(vector<string> Str){
    if (Str.size() != headers.size()) return -1;
    cells.push_back(Str);
    return (cells.size() + firstIndex - 1);
}

vector<string> Data::getColumn(string header){
    vector<string> column(cells.size());
    int num = Data::findHeaderColumn(header);
    for (int i = 0; i < cells.size(); i++){
        column[i] = cells[i][num];
    }
    return column;
}

string Data::getCell(string header, int num){
    int column = Data::findHeaderColumn(header);
    int innerNum = num - firstIndex;
    if ((innerNum < 0) || (innerNum > cells.size()) || (column == -1)) return "";
    return cells[innerNum][column];
}

int Data::size(){
    return cells.size();
}

string Data::serialize(){
    std::stringstream ss;
    vector<pair<string, string> > mainVector(cells.size() + 2);
    vector<pair<string, string> > headerPairs(headers.size());
    for (int i = 0; i < headers.size(); i++){
        ss.str("");
        ss << i;
        headerPairs[i] = pair<string, string>(ss.str(), headers[i]);
    }
    ss.str("");
    ss << firstIndex;
    mainVector[0] = pair<string, string>("headers", JsonMethods::toJsonString(headerPairs));
    mainVector[1] = pair<string, string>("firstIndex", ss.str());
    vector<pair<string, string> > stringPairs(headers.size());
    for (int i = 0; i < cells.size(); i++){
        for (int j = 0; j < cells[i].size(); j++){
            ss.str("");
            ss << j;
            stringPairs[j] = pair<string, string>(ss.str(), cells[i][j]);
        }
        ss.str("");
        ss << i;
        mainVector[i + 2] = pair<string, string>(ss.str(), JsonMethods::toJsonString(stringPairs));
    }
    string json = JsonMethods::toJsonString(mainVector);
    mainVector.clear();
    headerPairs.clear();
    stringPairs.clear();
    return json;
}

void Data::parse(string json){
    std::stringstream ss;
    vector<pair<string, string> > mainVector = JsonMethods::parseJson(json);
    vector<pair<string, string> > headerPairs;
    string firstIndexString = "0";
    int val;
    headers.clear();
    headers = *(new vector<string>(headerPairs.size()));
    cells.clear();
    if (mainVector.size() < 2) return;
    cells = *(new vector<vector<string> >(mainVector.size() - 2));
    for (int i = 0; i < mainVector.size(); i++){
        if (mainVector[i].first == "headers"){
            headerPairs = JsonMethods::parseJson(mainVector[i].second);
        }else if (mainVector[i].first == "firstIndex"){
            firstIndexString = mainVector[1].second;
        }else{
            val = 0;
            string key = mainVector[i].first;
            for (int q = key.size() - 1, k = 1; q >= 0; q--, k *= 10) {
                val += (key[q] - 48) * k;
            }
            vector<pair<string, string> > cellPairs = JsonMethods::parseJson(mainVector[i].second);
            cells[val] = *(new vector<string>(cellPairs.size()));
             val;
            for (int j = 0; j < cellPairs.size(); j++){
                int cellNum = 0;
                string cellNumString = cellPairs[j].first;
                for (int q = cellNumString.size() - 1, k = 1; q >= 0; q--, k *= 10) {
                    cellNum += (cellNumString[q] - 48) * k;
                }
                cells[val][cellNum] = cellPairs[j].second;
            }
            cellPairs.clear();
        }
    }
    headers = *(new vector<string>(headerPairs.size()));
    for (int i = 0; i < headerPairs.size(); i++){
        val = 0;
        string key = headerPairs[i].first;
        for (int i = key.size() - 1, k = 1; i >= 0; i--, k *= 10) {
            val += (key[i] - 48) * k;
        }
        headers[val] = headerPairs[i].second;
    }
    firstIndex = 0;
    for (int i = firstIndexString.size() - 1, k = 1; i >= 0; i--, k *= 10) {
        firstIndex += (firstIndexString[i] - 48) * k;
    }
    headerPairs.clear();
    mainVector.clear();
}


#endif // DATA

