#ifndef DBCLIENT_H
#define DBCLIENT_H

#include "HashTable/HashTable.h"
#include "HashTable/TableKey.h"
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "Data.h"
#include <QThreadPool>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QThread>


class DBClient: public QObject {
    Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    quint64     m_nNextBlockSize;
    QThread* thread;
    int maxCellsSize;
    //
public:
    bool put(short int userId, short int dataId, short int processId, MDPData* data);
    MDPData* get(short int userId, short int dataId, short int processId);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum);
    MDPData* get(short int userId, short int dataId, short int processId, int strNum, int count);
    bool remove(short int userId, short int dataId, short int processId);
    MDPData* getNextStrings(short int userId, short int dataId, short int processId, short int count);
    int getSize(short int userId, short int dataId, short int processId);
public:
    explicit DBClient(const QString& strHost, int nPort, QObject *parent = 0);
public slots:
    QJsonObject slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotConnected   (                            );
    void sendToServer(const QJsonObject& jso);
};

#endif // DBCLIENT_H
