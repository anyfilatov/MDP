#ifndef DISPATCHER_CPP
#define DISPATCHER_CPP

#include "Dispatcher/Dispatcher.h"
#include "Dispatcher/HardDiskWorker.h"
#include "HashTable/HashTable.h"
#include "HashTable/TableKey.h"
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "Data.h"
#include <QString>
#include <vector>
#include <fstream>
#include <QTextStream>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonArray>


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

void Dispatcher::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));

    qDebug() << "Dispatcher: new incoming connection from" << pClientSocket->peerAddress().toString() << ":" << pClientSocket->peerPort();
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
        qDebug() << comm;
        if (comm == "TO_START"){
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            toStart(userId, dataId, processId);

            jsAnswer.insert("COMMAND", "_TO_START");
            jsAnswer.insert("SUCCESS", true);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "ADD_HOST"){
            QString ip = jso.take("IP").toString();
            int port = jso.take("PORT").toInt();

            jsAnswer.insert("COMMAND", "_ADD_HOST");
            jsAnswer.insert("SUCCESS", true);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "PING_ALL"){
            QJsonArray ips;
            jsAnswer.insert("COMMAND", "_PING_ALL");
            jsAnswer.insert("HOSTS", ips);
            sendToClient(pClientSocket, jsAnswer);

        }else if (comm == "GET_TABLEIDS") {
            short userId = jso.take("USER_ID").toInt();
            QJsonArray ids = getTableIds(userId);
            jsAnswer.insert("COMMAND", "_GET_TABLEIDS");
            jsAnswer.insert("TABLE_IDS", ids);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "GET_USERS") {
            QJsonArray users = getUsers();
            jsAnswer.insert("COMMAND", "_GET_USERS");
            jsAnswer.insert("USERS", users);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "REMOVE") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            remove(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_REMOVE");
            jsAnswer.insert("SUCCESS", true);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "GET_SIZE") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            int size = getSize(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_GET_SIZE");
            jsAnswer.insert("SIZE", size);
            sendToClient(pClientSocket, jsAnswer);
        }else if (comm == "GET_NEXTSTR") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short count = jso.take("COUNT").toInt();
            d = getNextStrings(userId, dataId, processId, count);
            jsAnswer.insert("COMMAND", "_GET_NEXTSTR");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_NEXTSTR, user:" << userId << ", data:" << dataId << ", process:" << processId << ", count:" << count << ", " <<
                        ((d != NULL) ? ("firstIndex: "+ QString::number(d->getFirstIndex())) : ("DATA IS NULL"));
        } else if (comm == "GET_5") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short strNum = jso.take("STRNUM").toInt();
            short count = jso.take("COUNT").toInt();
            d = get(userId, dataId, processId, strNum, count);
            jsAnswer.insert("COMMAND", "_GET_5");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_5, user:" << userId << ", data:" << dataId << ", process:" << processId << ", num:" << strNum << ", count:" << count << ", " <<
                        ((d != NULL) ? ("firstIndex: "+ QString::number(d->getFirstIndex())) : ("DATA IS NULL"));
        } else if (comm == "GET_4") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            short strNum = jso.take("STR_NUM").toInt();
            d = get(userId, dataId, processId, strNum);
            jsAnswer.insert("COMMAND", "_GET_4");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_4, user:" << userId << ", data:" << dataId << ", process:" << processId << ", num:" << strNum << ", " <<
                        ((d != NULL) ? (d->serialize()) : ("DATA IS NULL"));
        } else if (comm == "GET_3") {
            short userId = jso.take("USER_ID").toInt();
            short dataId = jso.take("DATA_ID").toInt();
            short processId = jso.take("PROCESS_ID").toInt();
            d = get(userId, dataId, processId);
            jsAnswer.insert("COMMAND", "_GET_3");
            if (d != NULL) {
                jsAnswer.insert("DATA", d->serialize());
            }
            sendToClient(pClientSocket, jsAnswer);
            qDebug() << "Dispatcher: COMMAND GET_3, user:" << userId << ", data:" << dataId << ", process:" << processId << ", " <<
                        ((d != NULL) ? ("firstIndex: "+ QString::number(d->getFirstIndex())) : ("DATA IS NULL"));
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
            qDebug() << "Dispatcher: COMMAND PUT, user:" << userId << ", data:" << dataId << ", process:" << processId << ", SIZE : " << d->size();
            delete d;
        }

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

void Dispatcher::put(short int userId, short int dataId, short int processId, MDPData* data){
    HardDiskWorker::getInstance().put(userId, dataId, processId, data);
}

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    if (!HardDiskWorker::getInstance().contains(key)){
        return NULL;
    }
    int size = HardDiskWorker::getInstance().getSize(key);
    vector<QString> headers = HardDiskWorker::getInstance().getHeaders(key);
    vector<vector<QString> > cells(size);
    int count = size / HardDiskWorker::getInstance().getMaxFileSize() + 1;
    int line = 0;
    for (int i = 1; i <= count; i++){
        vector<vector<QString> > fileCells = HardDiskWorker::getInstance().get(userId, dataId, processId, i);
        for(int j = 0; j < fileCells.size(); j++){
            cells[line] = fileCells[j];
            line++;
        }
    }
    return new MDPData(headers, cells, 0);
}

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum){
    qDebug() << "GET_4 str:" << strNum;
    TableKey key(userId, dataId, processId);
    MDPData* cashData = cash.get(&key);
    int size = HardDiskWorker::getInstance().getSize(key);
    if ((!HardDiskWorker::getInstance().contains(key)) || (strNum < 0) || (strNum >= size)) return NULL;
    vector<QString> headers = HardDiskWorker::getInstance().getHeaders(key);
    if (!cashData){
        qDebug() << " new cashData";
        cashData = new MDPData();
        cashData->setHeaders(headers);
        cash.put(&key, cashData);
    }
    qDebug() << " headers size: " << headers.size();
    vector<vector<QString> > cells(1);
    if (!cashData->containsString(strNum)){
        int maxFileSize = HardDiskWorker::getInstance().getMaxFileSize();
        int currentPart = strNum / maxFileSize + 1;
        qDebug() << " put in cashData";
        qDebug() << " currentPart: " << currentPart;
        vector<vector<QString> > fileCells = HardDiskWorker::getInstance().get(userId, dataId, processId, currentPart);
        qDebug() << " first index: " << (strNum - strNum % maxFileSize);
        qDebug() << " cells size: " << fileCells.size();
        cashData->setCells(fileCells);
        cashData->setFirstIndex(strNum - strNum % maxFileSize);
    }
    cells[0] = cashData->getString(strNum);
    return new MDPData(headers, cells, strNum);
}

MDPData* Dispatcher::get(short int userId, short int dataId, short int processId, int strNum, int count){
    qDebug() << "GET_5 str:" << strNum << ", count: " << count;
    TableKey key(userId, dataId, processId);
    MDPData* cashData = cash.get(&key);
    int tableSize = HardDiskWorker::getInstance().getSize(key);
    if ((!HardDiskWorker::getInstance().contains(key)) || ((strNum + count) <= 0) || (strNum >= tableSize)) return NULL;
    vector<QString> headers = HardDiskWorker::getInstance().getHeaders(key);
    if (!cashData){
        qDebug() << " new cashData";
        cashData = new MDPData();
        cashData->setHeaders(headers);
        cash.put(&key, cashData);
    }
     qDebug() << " headers size: " << headers.size();
    int firstIndex = (strNum < 0)? 0 : strNum;
    int size = (((count + strNum) > tableSize)? tableSize : (count + strNum)) - firstIndex;
    vector<vector<QString> > cells(size);
    if ((!cashData->containsString(firstIndex)) || (!cashData->containsString(firstIndex + size - 1))){
        int maxFileSize = HardDiskWorker::getInstance().getMaxFileSize();
        int firstPart = strNum / maxFileSize + 1;
        int lastPart = (strNum + count - 1) / maxFileSize + 1;
        vector<vector<QString> > cashCells;
        for (int i = firstPart; i <= lastPart; i++){
            vector<vector<QString> > fileCells = HardDiskWorker::getInstance().get(userId, dataId, processId, i);
            for (int j = 0; j < fileCells.size(); j++){
                cashCells.push_back(fileCells[j]);
            }
        }
        qDebug() << "cells count: " << cashCells.size();
        cashData->setCells(cashCells);
        cashData->setFirstIndex(strNum - strNum % maxFileSize);
    }
    for (short int i = 0; i < size; i++){
        cells[i] = cashData->getString(firstIndex + i);
    }
    return new MDPData(headers, cells, firstIndex);
}

void Dispatcher::remove(short int userId, short int dataId, short int processId){

}

MDPData* Dispatcher::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    TableKey key(userId, dataId, processId);
    if (!HardDiskWorker::getInstance().contains(key)) return NULL;
    IntWithHash* increment = sessions.get(&key);
    if (!increment){
        increment = new IntWithHash(0);
        sessions.put(&key, increment);
    }
    int firstIndex = increment->getValue();
    if (increment->getValue() >= (HardDiskWorker::getInstance().getSize(key))){
        return NULL;
    }else{
        increment->setValue(increment->getValue() + count);
    }
    return get(userId, dataId, processId, firstIndex, count);
}

int Dispatcher::getSize(short userId, short dataId, short processId){
    TableKey key(userId, dataId, processId);
    return HardDiskWorker::getInstance().getSize(key);
}

void Dispatcher::toStart(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    if (!HardDiskWorker::getInstance().contains(key)) return;
    IntWithHash* increment = sessions.get(&key);
    if (!increment){
        increment = new IntWithHash(0);
        sessions.put(&key, increment);
    }else{
        increment->setValue(0);
    }
}

QJsonArray Dispatcher::getUsers(){
    QJsonArray array;
    vector<TableKey> keys = HardDiskWorker::getInstance().keys();
    for (int i = 0; i < keys.size(); i++){
        short userId = keys[i].getUserId();
        if (!array.contains(userId)){
            array.push_back(userId);
        };
    }
    return array;
}

QJsonArray Dispatcher::getTableIds(short userId){
    QJsonArray array;
    vector<TableKey> keys = HardDiskWorker::getInstance().keys();
    for (int i = 0; i < keys.size(); i++){
        short currentId = keys[i].getUserId();
        if (userId == currentId){
            short dataId = keys[i].getDataId();
            short processId = keys[i].getProcessId();
            QJsonObject key;
            key.insert("DATA_ID", dataId);
            key.insert("PROCESS_ID", processId);
            array.push_back(key);
        }
    }
    return array;
}

#endif
