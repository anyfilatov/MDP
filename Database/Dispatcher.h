#ifndef DISPATCHER
#define DISPATCHER

#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "Data.h"
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include<QTextStream>

using namespace std;
class Dispatcher{
private:
    HashTable<TableKey, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> > > hashTable;
    HashTable<TableKey, pair<int, int> > tableInfo;
    HashTable<TableKey, IntWithHash> sessions;
    void putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table);
    int updateTableInfo(TableKey* key, int num);
public:
    void put(short int userId, short int dataId, short int processId, Data* data);
    Data* get(short int userId, short int dataId, short int processId);
    Data* get(short int userId, short int dataId, short int processId, int strNum);
    Data* get(short int userId, short int dataId, short int processId, int strNum, int count);
    //void updateString(short int userId, short int dataId, short int processId, int strNum, vector<string> data);
    void remove(short int userId, short int dataId, short int processId);
    Data* getNextStrings(short int userId, short int dataId, short int processId, short int count);
};

void Dispatcher::putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table){
    vector<string> headers = data->getHeaders();
    vector<vector<string> > cells = data->getCells();
    for (short int i = 0; i < headers.size(); i++){
        StringWithHash header(headers[i]);
        HashTable<IntWithHash, StringWithHash>* innerTable = table->get(&header);
        if (!innerTable){
            innerTable = new HashTable<IntWithHash, StringWithHash>();
            table->put(&header, innerTable);
        }
        for (int j = 0; j < cells.size(); j++){
            IntWithHash strNum(data->getFirstIndex() + j);
            if (cells[i].size() >= j){
                StringWithHash* cell = innerTable->get(&strNum);
                if (!cell){
                    cell = new StringWithHash(cells[i][j]);
                    innerTable->put(&strNum, cell);
                }else{
                    cell->setValue(cells[i][j]);
                }
            }
        }
    }
    headers.clear();
    cells.clear();
}

int Dispatcher::updateTableInfo(TableKey* key, int num){
    pair<int, int>* Pair = tableInfo.get(key);
    if (!Pair){
        Pair = new  pair<int, int>(0, num);
        tableInfo.put(key, Pair);
        return 0;
    }
    int first = Pair->first + Pair->second;
    Pair->second += num;
    return first;
}

void Dispatcher::put(short int userId, short int dataId, short int processId, Data* data){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    if (!table){
        table = new HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >();
        hashTable.put(&key, table);
    }
    int firstIndex = updateTableInfo(&key, data->size());
    data->setFirstIndex(firstIndex);
    putDataInHashTable(data, table);
}

Data* Dispatcher::get(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    if (!table) return NULL;
    vector<StringWithHash*> keys = table->keys();
    pair<int, int>* info = tableInfo.get(&key);
    vector<string> headers(keys.size());
    vector<vector<string> > cells(info->second);
    for (short int i = 0; i < keys.size(); i++){
        string header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < info->second; j++){
            string cell;
            if (!column){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            IntWithHash intKey(info->first + j);
            StringWithHash* value = column->get(&intKey);
            if (!value){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            cell = value->getValue();
            cells[j].push_back(cell);
        }
    }
    return new Data(headers, cells, info->first);
}

Data* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || (strNum < info->first) || (strNum >= (info->first + info->second))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<string> headers(keys.size());
    vector<vector<string> > cells(1);
    for (short int i = 0; i < keys.size(); i++){
        string header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        string cell;
        if (!column){
            cell = "";
            cells[0].push_back(cell);
            continue;
        }
        IntWithHash intKey(strNum);
        StringWithHash* value = column->get(&intKey);
        if (!value){
            cell = "";
            cells[0].push_back(cell);
            continue;
        }
        cell = value->getValue();
        cells[0].push_back(cell);
    }
    return new Data(headers, cells, strNum);
}

Data* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum, int count){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || ((strNum + count) <= info->first) || (strNum >= (info->first + info->second))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<string> headers(keys.size());
    int firstIndex = (strNum < info->first)? info->first : strNum;
    int size = (((count + strNum) > (info->second + info->first))? (info->second + info->first) : (count + strNum)) - firstIndex;
    vector<vector<string> > cells(size);
    for (short int i = 0; i < keys.size(); i++){
        string header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < size; j++){
            string cell;
            if (!column){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            IntWithHash intKey(firstIndex + j);
            StringWithHash* value = column->get(&intKey);
            if (!value){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            cell = value->getValue();
            cells[j].push_back(cell);
        }
    }
    return new Data(headers, cells, firstIndex);
}

void Dispatcher::remove(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    hashTable.remove(&key);
    tableInfo.remove(&key);
}

Data* Dispatcher::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    TableKey key(userId, dataId, processId);
    IntWithHash* increment = sessions.get(&key);
    if (!increment){
        increment = new IntWithHash(0);
        sessions.put(&key, increment);
    }
    int firstIndex = increment->getValue();
    pair<int, int>* info = tableInfo.get(&key);
    if ((increment->getValue() + count) >= (info->first + info->second)){
        sessions.remove(&key);
    }else{
        increment->setValue(increment->getValue() + count);
    }
    return get(userId, dataId, processId, firstIndex, count);
}

#endif // DISPATCHER

