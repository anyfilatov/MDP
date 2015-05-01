#ifndef TABLEKEY
#define TABLEKEY

#include "AbstractTableKey.h"
#include <string>
#include "JsonMethods.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class TableKey: public AbstractTableKey{
private:
    int userId;
    int dataId;
    unsigned int processId;
public:
    TableKey();
    TableKey(int userId, int dataId);
    TableKey(int userId, int dataId, int processId);
    ~TableKey();
    int getUserId();
    void setUserId(int userId);
    int getDataId();
    void setDataId(int dataId);
    int getProcessId();
    void setProcessId(int processId);
    void setKey(int userId, int dataId, int processId);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    string serialize();
    void parse(string json);
};

TableKey::TableKey(){
    userId = -1;
    dataId = -1;
    processId = 0;
}

TableKey::TableKey(int userId, int dataId){
    this(userId, dataId, 0);
}

TableKey::TableKey(int userId, int dataId, int processId){
    this->userId = userId;
    this->dataId = dataId;
    this->processId = processId;
}

TableKey::~TableKey(){
}

int TableKey::getUserId(){
    return userId;
}

void TableKey::setUserId(int userId){
    this->userId = userId;
}

int TableKey::getDataId(){
    return dataId;
}

void TableKey::setDataId(int dataId){
    this->dataId = dataId;
}

int TableKey::getProcessId(){
    return processId;
}

void TableKey::setProcessId(int processId){
    this->processId = processId;
}

void TableKey::setKey(int userId, int dataId, int processId){
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

string TableKey::serialize(){
    std::stringstream ss;
    ss << "{\"userId\":\"" << userId << "\",\"dataId\":\"" << dataId << "\",\"processId\":\"" << processId << "\"}";
    return ss.str();
}

void TableKey::parse(string json){
     vector<pair<string, string> > pairs = JsonMethods.parseJson(json);
     for (int i = 0; i < pairs.size(); i++){
         pair<string, string> Pair = pairs[i];
         if (Pair.first == "userId"){
             userId = Pair.second;
         } else if (Pair.first == "dataId"){
             dataId = Pair.second;
         } else if (Pair.first == "processId"){
             processId = Pair.second;
         }
     }
     pairs.clear();
}

#endif // TABLEKEY

