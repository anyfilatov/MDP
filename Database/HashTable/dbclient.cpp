#include "DBClient.h"
#include "HashTable.h"
#include "TableKey.h"
#include "StringWithHash.h"
#include "IntWithHash.h"
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


using namespace std;

DBClient::DBClient(const QString& strHost, int nPort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    //connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    qDebug() << "DBClient: Started; server host: " << strHost << ", port: " << nPort;
}

QJsonObject DBClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (m_pTcpSocket->waitForReadyRead(10000)){
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
    qDebug() << "DBClient: sended to server -> " << jsDoc.toJson();
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
    if (m_pTcpSocket->waitForReadyRead(5000)){
        QJsonObject obj = slotReadyRead();
        MDPData data;
        if (obj.contains("SUCCESS")){
            return obj.take("SUCCESS").toBool();
        }else{
            return false;
        }
    }
    return false;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_3");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    sendToServer(obj);
        QJsonObject obj2 = slotReadyRead();
        QJsonDocument doc(obj2);
        qDebug() << doc.toJson();
        /*MDPData* data = new MDPData;

        if (obj.contains("DATA")){
            qDebug() << obj.take("DATA").toString();
            //data->parse(obj.take("DATA").toString());
            //data->serialize();
            return data;
        }else{
            return NULL;
        }*/
    return NULL;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId, int strNum){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_4");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("STR_NUM", strNum);
    sendToServer(obj);
    if (m_pTcpSocket->waitForReadyRead(5000)){
        QJsonObject obj = slotReadyRead();
        MDPData* data = new MDPData;
        if (obj.contains("DATA")){
            data->parse(obj.take("DATA").toString());
            data->serialize();
            return data;
        }else{
            return NULL;
        }
    }
    return NULL;
}

MDPData* DBClient::get(short int userId, short int dataId, short int processId, int strNum, int count){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_5");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("STR_NUM", strNum);
    obj.insert("COUNT", count);
    sendToServer(obj);
    if (m_pTcpSocket->waitForReadyRead(5000)){
        QJsonObject obj = slotReadyRead();
        MDPData* data = new MDPData;
        if (obj.contains("DATA")){
            data->parse(obj.take("DATA").toString());
            data->serialize();
            return data;
        }else{
            return NULL;
        }
    }
    return NULL;
}

void DBClient::remove(short int userId, short int dataId, short int processId){

}

MDPData* DBClient::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    QJsonObject obj;
    obj.insert("COMMAND", "GET_NEXTSTR");
    obj.insert("USER_ID", userId);
    obj.insert("DATA_ID", dataId);
    obj.insert("PROCESS_ID", processId);
    obj.insert("COUNT", count);
    sendToServer(obj);
    if (m_pTcpSocket->waitForReadyRead(5000)){
        QJsonObject obj = slotReadyRead();
        MDPData* data = new MDPData;
        if (obj.contains("DATA")){
            data->parse(obj.take("DATA").toString());
            data->serialize();
            return data;
        }else{
            return NULL;
        }
    }
    return NULL;
}
