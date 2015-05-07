#ifndef COMMONSTORAGE_H
#define COMMONSTORAGE_H

#include "HashTable.h"
#include "Data.h"

class CommonStorage
{
protected:
    HashTable<TableKey, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> > > hashTable;
    HashTable<TableKey, pair<int, int> > tableInfo;
    HashTable<TableKey, IntWithHash> sessions;
public:
    void putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table);
    int updateTableInfo(TableKey* key, int num);
    void put(short int userId, short int dataId, short int processId, Data* data);
    Data* get(short int userId, short int dataId, short int processId);
    Data* get(short int userId, short int dataId, short int processId, int strNum);
    Data* get(short int userId, short int dataId, short int processId, int strNum, int count);
    //void updateString(short int userId, short int dataId, short int processId, int strNum, vector<QString> data);
    void remove(short int userId, short int dataId, short int processId);
    Data* getNextStrings(short int userId, short int dataId, short int processId, short int count);
};

#endif // COMMONSTORAGE_H
