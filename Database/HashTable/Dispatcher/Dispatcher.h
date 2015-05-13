#ifndef DISPATCHER_H
#define DISPATCHER_H


#include "HashTable/HashTable.h"
#include "HashTable/TableKey.h"
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "Data.h"
#include <QObject>
#include <QTcpServer>
#include <QJsonArray>

using namespace std;

class Dispatcher: public QObject{
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    quint64     m_nNextBlockSize;
    void sendToClient(QTcpSocket* pSocket, const QJsonObject& jso);
public:
    explicit Dispatcher(int nPort, QObject *parent = 0);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
private:
    HashTable<TableKey, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> > > hashTable;
    HashTable<TableKey, IntWithHash > tableInfo;
    HashTable<TableKey, IntWithHash> sessions;
    void putDataInHashTable(MDPData* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table);
    int updateTableInfo(TableKey* key, int num);
public:
    void put(short int userId, short int dataId, short int processId, MDPData* data);
    MDPData* get(short int userId, short int dataId, short int processId);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum, int count);
    void remove(short int userId, short int dataId, short int processId);
    MDPData* getNextStrings(short int userId, short int dataId, short int processId, short int count);
    int getSize(short int userId, short int dataId, short int processId);
    QJsonArray getUsers();
};


#endif // DISPATCHER

