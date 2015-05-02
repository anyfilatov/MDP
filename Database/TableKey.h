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
    short int userId;
    short int dataId;
    short int processId;
public:
    TableKey();
    TableKey(short int userId, short int dataId);
    TableKey(short int userId, short int dataId, short int processId);
    ~TableKey();
    short int getUserId();
    void setUserId(short int userId);
    short int getDataId();
    void setDataId(short int dataId);
    short int getProcessId();
    void setProcessId(short int processId);
    void setKey(short int userId, short int dataId, short int processId);
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

short int TableKey::getUserId(){
    return userId;
}

void TableKey::setUserId(short int userId){
    this->userId = userId;
}

short int TableKey::getDataId(){
    return dataId;
}

void TableKey::setDataId(short int dataId){
    this->dataId = dataId;
}

short int TableKey::getProcessId(){
    return processId;
}

void TableKey::setProcessId(short int processId){
    this->processId = processId;
}

void TableKey::setKey(short int userId, short int dataId, short int processId){
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
     vector<pair<string, string> > pairs = JsonMethods::parseJson(json);
     for (int i = 0; i < pairs.size(); i++){
         pair<string, string> Pair = pairs[i];
         if (Pair.first == "userId"){
             userId = JsonMethods::getInt(Pair.second);
         } else if (Pair.first == "dataId"){
             dataId = JsonMethods::getInt(Pair.second);
         } else if (Pair.first == "processId"){
             processId = JsonMethods::getInt(Pair.second);
         }
     }
     pairs.clear();
}



#endif // TABLEKEY

