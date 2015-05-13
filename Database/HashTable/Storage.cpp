#ifndef STORAGE_CPP
#define STORAGE_CPP

#include "Storage.h"
#include "HashTable.h"
#include "HashTable/TableKey.h"
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "Data.h"
#include <QString>
#include <vector>
//#include <conio.h>
#include <fstream>
#include <QTextStream>
#include <QObject>
#include <QTcpServer>
#include <QDebug>


using namespace std;

void Storage::putDataInHashTable(MDPData* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table){
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

int Storage::updateTableInfo(TableKey* key, int num){
    IntWithHash* Info = tableInfo.get(key);
    if (!Info){
        Info = new  IntWithHash(num);
        tableInfo.put(key, Info);
        return 0;
    }
    int first = Info->getValue();
    Info->setValue(first + num);
    return first;
}

void Storage::put(short int userId, short int dataId, short int processId, MDPData* data){
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

MDPData* Storage::get(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    if (!table) return NULL;
    vector<StringWithHash*> keys = table->keys();
    IntWithHash* info = tableInfo.get(&key);
    vector<QString> headers(keys.size());
    vector<vector<QString> > cells(info->getValue());
    for (short int i = 0; i < keys.size(); i++){
        QString header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < info->getValue(); j++){
            QString cell;
            if (!column){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            IntWithHash intKey(j);
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
    return new MDPData(headers, cells, 0);
}

MDPData* Storage::get(short int userId, short int dataId, short int processId, int strNum){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    IntWithHash* info = tableInfo.get(&key);
    if ((!table) || (strNum < 0) || (strNum >= info->getValue())) return NULL;
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
    return new MDPData(headers, cells, strNum);
}

MDPData* Storage::get(short int userId, short int dataId, short int processId, int strNum, int count){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    IntWithHash* info = tableInfo.get(&key);
    if ((!table) || ((strNum + count) <= 0) || (strNum >= (info->getValue()))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<QString> headers(keys.size());
    int firstIndex = (strNum < 0)? 0 : strNum;
    int size = (((count + strNum) > (info->getValue()))? (info->getValue()) : (count + strNum)) - firstIndex;
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
    return new MDPData(headers, cells, firstIndex);
}

void Storage::remove(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    hashTable.remove(&key);
    tableInfo.remove(&key);
}

#endif
