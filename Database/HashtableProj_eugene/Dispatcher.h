#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "Data.h"
#include <QThreadPool>
#include "QTcpServer"
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>

using namespace std;

class Dispatcher: public QObject {
    Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    quint16     m_nNextBlockSize;
    void sendToClient(QTcpSocket* pSocket, const QByteArray& str);
public:
    explicit Dispatcher(int nPort, QObject *parent = 0);

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();

    //
private:
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



#endif // DISPATCHER

