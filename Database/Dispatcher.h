#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "Data.h"
#include <QThreadPool>
#include "QTcpServer"

using namespace std;

class Dispatcher: public QTcpServer{
    Q_OBJECT
private:
    QThreadPool *pool;
    HashTable<TableKey, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> > > hashTable;
    HashTable<TableKey, pair<int, int> > tableInfo;
    HashTable<TableKey, IntWithHash> sessions;
    void putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table);
    int updateTableInfo(TableKey* key, int num);
    void put(short int userId, short int dataId, short int processId, Data* data);
    Data* get(short int userId, short int dataId, short int processId);
    Data* get(short int userId, short int dataId, short int processId, int strNum);
    Data* get(short int userId, short int dataId, short int processId, int strNum, int count);
    //void updateString(short int userId, short int dataId, short int processId, int strNum, vector<QString> data);
    void remove(short int userId, short int dataId, short int processId);
    Data* getNextStrings(short int userId, short int dataId, short int processId, short int count);
    //void incomingConnection( qintptr handle );
public:
    explicit Dispatcher(QObject *parent = 0);
    void startServer(int port);
};



#endif // DISPATCHER

