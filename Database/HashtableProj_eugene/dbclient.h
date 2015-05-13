#ifndef DBCLIENT_H
#define DBCLIENT_H

#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "Data.h"
#include <QThreadPool>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>


class DBClient: public QObject {
    Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    quint16     m_nNextBlockSize;

    //

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
public:
    explicit DBClient(const QString& strHost, int nPort, QObject *parent = 0);
public slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   (                            );
    void sendToServer(const QJsonObject& jso);
};

#endif // DBCLIENT_H
