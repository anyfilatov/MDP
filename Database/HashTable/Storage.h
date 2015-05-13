#ifndef STORAGE
#define STORAGE

#include "HashTable/HashTable.h"
#include "HashTable/TableKey.h"
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "Data.h"

using namespace std;

class Storage{
private:
    bool condition;
    HashTable<TableKey, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> > > hashTable;
    HashTable<TableKey, IntWithHash > tableInfo;
    void putDataInHashTable(MDPData* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table);
    int updateTableInfo(TableKey* key, int num);

    //void incomingConnection( qintptr handle );
public:
    void put(short int userId, short int dataId, short int processId, MDPData* data);
    MDPData* get(short int userId, short int dataId, short int processId);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum, int count);
    //void updateString(short int userId, short int dataId, short int processId, int strNum, vector<QString> data);
    void remove(short int userId, short int dataId, short int processId);
};

#endif // STORAGE

