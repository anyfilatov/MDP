#ifndef COMMONSTORAGE_CPP
#define COMMONSTORAGE_CPP

#include "commonstorage.h"
#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "Data.h"
#include <QString>
#include <vector>
#include <conio.h>
#include <fstream>
#include <QTextStream>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

void CommonStorage::putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table){
    vector<QString> headers = data->getHeaders();
    vector<vector<QString> > cells = data->getCells();
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

int CommonStorage::updateTableInfo(TableKey* key, int num){
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

void CommonStorage::put(short int userId, short int dataId, short int processId, Data* data){
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

Data* CommonStorage::get(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    if (!table) return NULL;
    vector<StringWithHash*> keys = table->keys();
    pair<int, int>* info = tableInfo.get(&key);
    vector<QString> headers(keys.size());
    vector<vector<QString> > cells(info->second);
    for (short int i = 0; i < keys.size(); i++){
        QString header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < info->second; j++){
            QString cell;
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

Data* CommonStorage::get(short int userId, short int dataId, short int processId, int strNum){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || (strNum < info->first) || (strNum >= (info->first + info->second))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<QString> headers(keys.size());
    vector<vector<QString> > cells(1);
    for (short int i = 0; i < keys.size(); i++){
        QString header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        QString cell;
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

Data* CommonStorage::get(short int userId, short int dataId, short int processId, int strNum, int count){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || ((strNum + count) <= info->first) || (strNum >= (info->first + info->second))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<QString> headers(keys.size());
    int firstIndex = (strNum < info->first)? info->first : strNum;
    int size = (((count + strNum) > (info->second + info->first))? (info->second + info->first) : (count + strNum)) - firstIndex;
    vector<vector<QString> > cells(size);
    for (short int i = 0; i < keys.size(); i++){
        QString header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < size; j++){
            QString cell;
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

void CommonStorage::remove(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    hashTable.remove(&key);
    tableInfo.remove(&key);
}

Data* CommonStorage::getNextStrings(short int userId, short int dataId, short int processId, short int count){
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

#endif
