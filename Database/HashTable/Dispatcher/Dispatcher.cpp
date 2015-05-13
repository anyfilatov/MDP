#ifndef DISPATCHER_CPP
#define DISPATCHER_CPP

#include "Dispatcher.h"
#include "HashTable/HashTable.h"
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
#include <QTcpSocket>
#include <QDebug>


using namespace std;

Dispatcher::Dispatcher(int nPort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        qDebug() << "Dispatcher: Unable to start the server:" << m_ptcpServer->errorString();
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    qDebug() << "Dispatcher: Listening port " << nPort << " started";
}

/*virtual*/ void Dispatcher::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    qDebug() << "Dispatcher: new incoming connection from" << pClientSocket->peerAddress().toString() << ":" << pClientSocket->peerPort();

    //sendToClient(pClientSocket, "Server Response: Connected!");
}

void Dispatcher::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint64)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        QByteArray arr;
        in >> arr;

        QJsonObject jso = QJsonDocument::fromJson(arr).object();
        QJsonObject jsAnswer;

        MDPData* d;
        QString comm = jso.take("COMMAND").toString();

        if (comm == "REMOVE") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            int size = getSize(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_REMOVE");
            TableKey key(userId, dataId, processId);
            IntWithHash* info = tableInfo.get(&key);
            if (info != NULL){
                remove(userId, dataId, processId);
                jsAnswer.insert("SUCCESS", true);
            }else{
                jsAnswer.insert("SUCCESS", false);
            }
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "GET_SIZE") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            int size = getSize(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_GET_SIZE");
            if (d >= 0) {
                jsAnswer.insert("SIZE", size);
            }
            sendToClient(pClientSocket, jsAnswer);
           //qDebug() << "Dispatcher: COMMAND GET_3 " << userId << ":" << dataId << ":" << processId << ":" << "; SIZE : " << d->size();
        }else if (comm == "GET_NEXTSTR") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short count = jso.take("COUNT").toInt();
            d = getNextStrings(userId, dataId, processId, count);
            jsAnswer.insert("COMMAND", "_GET");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_NEXTSTR " << userId << ":" << dataId << ":" << processId << ":" << count << ":" <<
                        ((d != NULL) ? (d->serialize()) : ("DATA IS NULL"));
        } else if (comm == "GET_5") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short strNum = jso.take("STRNUM").toInt();
            short count = jso.take("COUNT").toInt();
            d = get(userId, dataId, processId, strNum, count);
            jsAnswer.insert("COMMAND", "_GET");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_5 " << userId << ":" << dataId << ":" << processId << ":" << strNum << ":" << count << ":" <<
                        ((d != NULL) ? (d->serialize()) : ("DATA IS NULL"));
        } else if (comm == "GET_4") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short strNum = jso.take("STRNUM").toInt();
            d = get(userId, dataId, processId, strNum);
            jsAnswer.insert("COMMAND", "_GET");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_4 " << userId << ":" << dataId << ":" << processId << ":" << strNum << ":" <<
                        ((d != NULL) ? (d->serialize()) : ("DATA IS NULL"));
        } else if (comm == "GET_3") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            d = get(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_GET");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_3 " << userId << ":" << dataId << ":" << processId << ":" <<
                        ((d != NULL) ? (d->serialize()) : ("DATA IS NULL"));
            //qDebug() << "Dispatcher: COMMAND GET_3 " << userId << ":" << dataId << ":" << processId << ":" << "; SIZE : " << d->size();
        } else if (comm == "PUT") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            d = new MDPData;
            d->parse(jso.take("DATA").toString());
            put(userId, dataId, processId, d);
            jsAnswer.insert("COMMAND", "_PUT");
            if (d != NULL) {
                jsAnswer.insert("SUCCESS", true);
            }
            sendToClient(pClientSocket, jsAnswer);
            //qDebug() << "Dispatcher: COMMAND PUT " << userId << ":" << dataId << ":" << processId << ":" << d->serialize();
            qDebug() << "Dispatcher: COMMAND PUT " << userId << ":" << dataId << ":" << processId << "; SIZE : " << d->size();
            delete d;
        }


        //qDebug() << "Dispatcher: Incoming message : " << arr;
        //qDebug() << "Dispatcher: Command -> " << comm;
        //qDebug() << "Dispatcher: Data -> " << d.serialize();

        m_nNextBlockSize = 0;
    }
}

void Dispatcher::sendToClient(QTcpSocket* pSocket, const QJsonObject& jso)
{
    QByteArray  arrBlock;

    QJsonDocument jsDoc(jso);

    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) << jsDoc.toJson();;

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    pSocket->write(arrBlock);
}

//-------------------------------

void Dispatcher::putDataInHashTable(MDPData* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table){
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
            if (cells[j].size() >= i){
                StringWithHash* cell = innerTable->get(&strNum);
                if (!cell){
                    cell = new StringWithHash(cells[j][i]);
                    innerTable->put(&strNum, cell);
                }else{
                    cell->setValue(cells[j][i]);
                }
            }
        }
    }
    headers.clear();
    cells.clear();
}

int Dispatcher::updateTableInfo(TableKey* key, int num){
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

void Dispatcher::put(short int userId, short int dataId, short int processId, MDPData* data){
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

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId){
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

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum){
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

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum, int count){
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

void Dispatcher::remove(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    hashTable.remove(&key);
    tableInfo.remove(&key);
}

MDPData* Dispatcher::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    TableKey key(userId, dataId, processId);
    IntWithHash* increment = sessions.get(&key);
    if (!increment){
        increment = new IntWithHash(0);
        sessions.put(&key, increment);
    }
    int firstIndex = increment->getValue();
    IntWithHash* info = tableInfo.get(&key);
    if ((increment->getValue() + count) >= (info->getValue())){
        sessions.remove(&key);
    }else{
        increment->setValue(increment->getValue() + count);
    }
    return get(userId, dataId, processId, firstIndex, count);
}

int Dispatcher::getSize(short userId, short dataId, short processId){
    TableKey key(userId, dataId, processId);
    IntWithHash* info = tableInfo.get(&key);
    if (info != NULL){
        return info->getValue();
    }
    return 0;
}

#endif
