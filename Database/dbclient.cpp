#ifndef DBCLIENT_CPP
#define DBCLIENT_CPP

#include "dbclient.h"
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
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <QtDebug>
using namespace std;

DBClient::DBClient(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    maxCellsSize = 1000;
    tryNum = 0;
    ip = strHost;
    spareIp = strSpareHost;
    port = nPort;
    sparePort = nSparePort;
    _connect();

}

void DBClient::_connect(){
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(ip, port);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    //connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    qDebug() << "DBClient: Started; server host: " << ip << ", port: " << port;
}

void DBClient::switchIps(){
    QString str = ip;
    ip = spareIp;
    spareIp = str;
    int n = port;
    port = sparePort;
    sparePort = n;
}

void DBClient::setIps(const QString &strHost, int nPort, const QString &strSpareHost, int nSparePort){
    ip = strHost;
    spareIp = strSpareHost;
    port = nPort;
    sparePort = nSparePort;
    _connect();
}

QJsonObject DBClient::slotReadyRead(QJsonObject &obj)
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (m_pTcpSocket->waitForReadyRead(600000)){
            if (!m_nNextBlockSize) {
                if (m_pTcpSocket->bytesAvailable() < sizeof(quint64)) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
                continue;
            }else
                break;
        }else{
            if (tryNum > 20){
                QJsonObject jso;
                jso.insert("TIME_OUT", true);
                return jso;
            }

            m_pTcpSocket->close();
            delete m_pTcpSocket;
            switchIps();
            _connect();
            qDebug() << "RECONNECT";
            sendToServer(obj);
            tryNum++;
            return slotReadyRead(obj);
        }
    }
    QByteArray arr;
    in >> arr;
    m_nNextBlockSize = 0;
    QJsonObject jso = QJsonDocument::fromJson(arr).object();
    return jso;
}

void DBClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    qDebug() << "DBClient: ERROR: " << strError;
}

void DBClient::sendToServer(const QJsonObject& jso)
{
    QJsonDocument jsDoc(jso);

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) << jsDoc.toJson();

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    m_pTcpSocket->write(arrBlock);
  //  qDebug() << "DBClient: sended to server -> " << jsDoc.toJson();
}

void DBClient::slotConnected()
{
    qDebug() << "DBClient: Connect success";
}

//end of



bool DBClient::put(short int userId, short int dataId, short int processId, MDPData* data){
    QJsonObject obj;
    obj.insert("COMMAND", "PUT");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("DATA", data->serialize());
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return false;
    }
    if (obj2.contains("SUCCESS")){
        return obj2.take("SUCCESS").toBool();
    }
    return false;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId){
    int size = getSize(userId, dataId, processId);
    qDebug() << size;
    if (size <= maxCellsSize){
        QJsonObject obj;
        obj.insert("COMMAND", "GET_3");
        obj.insert("USER_ID", userId);
        obj.insert("DATA_ID", dataId);
        obj.insert("PROCESS_ID", processId);
        sendToServer(obj);
        tryNum = 0;
        QJsonObject obj2 = slotReadyRead(obj);
        if (obj2.contains("TIME_OUT")){
            return NULL;
        }
        QJsonDocument doc(obj2);
        qDebug() << doc.toJson();
        MDPData* data = new MDPData;
        if (obj2.contains("DATA")){
            data->parse(obj2.take("DATA").toString());
            return data;
        }
        return NULL;
    }
    int first = 0;
    vector<vector<QString> > allCells;
    vector<QString> headers;

    while(first < size){
        MDPData* data = get(userId, dataId, processId, first, maxCellsSize);
        if (headers.size() == 0){
            headers = data->getHeaders();
        }
        vector<vector<QString> > cells = data->getCells();
        for (int i = 0; i < cells.size(); i++){
            allCells.push_back(cells[i]);
        }
        first += maxCellsSize;
    }

    MDPData* allData = new MDPData(headers, allCells, 0);
    return allData;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId, int strNum){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_4");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("STR_NUM", strNum);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return NULL;
    }
    MDPData* data = new MDPData;
    if (obj2.contains("DATA")){
        data->parse(obj2.take("DATA").toString());
        return data;
    }
    return NULL;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId, int strNum, int count){
    if (count <= maxCellsSize){
        QJsonObject obj;
        obj.insert("COMMAND", "GET_5");
        obj.insert("USER_ID", userId);
        obj.insert("DATA_ID", dataId);
        obj.insert("PROCESS_ID", processId);
        obj.insert("STR_NUM", strNum);
        obj.insert("COUNT", count);
        sendToServer(obj);
        tryNum = 0;
        QJsonObject obj2 = slotReadyRead(obj);
        if (obj2.contains("TIME_OUT")){
            return NULL;
        }
        MDPData* data = new MDPData;
        if (obj2.contains("DATA")){
            data->parse(obj2.take("DATA").toString());
            return data;
        }
        return NULL;
    }
    int first = strNum;
    vector<vector<QString> > allCells;
    vector<QString> headers;

    while(first < strNum + count){
        MDPData* data = get(userId, dataId, processId, first, maxCellsSize);
        if (headers.size() == 0){
            headers = data->getHeaders();
        }
        vector<vector<QString> > cells = data->getCells();
        for (int i = 0; i < cells.size(); i++){
            allCells.push_back(cells[i]);
        }
        if (data){
            qDebug() << data->serialize();
        }
        first += maxCellsSize;
    }

    MDPData* allData = new MDPData(headers, allCells, strNum);
    return allData;
}

bool DBClient::remove(short int userId, short int dataId, short int processId){
    QJsonObject obj;
    obj.insert("COMMAND", "REMOVE");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return false;
    }
    if (obj2.contains("SUCCESS")){
        return obj2.take("SUCCESS").toBool();
    }
    return false;
}

MDPData* DBClient::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_NEXTSTR");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("COUNT", count);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return NULL;
    }
    MDPData* data = new MDPData;
    if (obj2.contains("DATA")){
        data->parse(obj2.take("DATA").toString());
        return data;
    }
    return NULL;
}

bool DBClient::toStart(short userId, short dataId, short processId){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_SIZE");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return false;
    }
    if (obj2.contains("SUCCESS")){
        return obj2.take("SUCCESS").toBool();
    }
    return false;
}

int DBClient::getSize(short userId, short dataId, short processId){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_SIZE");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        return 0;
    }
    if (obj2.contains("SIZE")){
        return obj2.take("SIZE").toInt();
    }
    return 0;
}

vector<short> DBClient::getUsers(){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_USERS");
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    vector<short> users;
    if (obj2.contains("TIME_OUT")){
        return users;
    }
    if (obj2.contains("USERS")){
        QJsonArray array = obj2.take("USERS").toArray();
        for (short i = 0; i < array.size(); i++){
            users.push_back(array[i].toInt());
        }
    }
    return users;
}


vector<pair<short, short> > DBClient::getTableIds(short userId){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_TABLEIDS");
    obj.insert("USER_ID", userId);
    sendToServer(obj);
    tryNum = 0;
    QJsonObject obj2 = slotReadyRead(obj);
    vector<pair<short, short> > ids;
    if (obj2.contains("TIME_OUT")){
        return ids;
    }
    if (obj.contains("TABLE_IDS")){
        QJsonArray array = obj.take("TABLE_IDS").toArray();
        qDebug() << array;
        for (short i = 0; i < array.size(); i++){
            QJsonObject obj = array[i].toObject();
            short dataId = obj.take("DATA_ID").toInt();
            short processId = obj.take("PROCESS_ID").toInt();
            pair<short, short> Pair(dataId, processId);
            ids.push_back(Pair);
        }
    }
    return ids;
}

#endif
