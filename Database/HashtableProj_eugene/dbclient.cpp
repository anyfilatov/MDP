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


using namespace std;

DBClient::DBClient(const QString& strHost, int nPort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    qDebug() << "DBClient: Started; server host: " << strHost << ", port: " << nPort;
}

void DBClient::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint64)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QByteArray arr;
        in >> arr;

        qDebug() << "DBClient: message from server:" << arr;
        m_nNextBlockSize = 0;
    }
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
    out << quint16(0) << jsDoc.toJson();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void DBClient::slotConnected()
{
    qDebug() << "DBClient: Connect success";
}

//end of

void DBClient::putDataInHashTable(Data* data, HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table){
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
            if (cells[i].size() >= j){
                StringWithHash* cell = innerTable->get(&strNum);
                if (!cell){
                    cell = new StringWithHash(cells[i][j]);
                    innerTable->put(&strNum, cell);
                }else{
                    cell->setValue(cells[i][j]);
                }
            }
        }
    }
    headers.clear();
    cells.clear();
}

int DBClient::updateTableInfo(TableKey* key, int num){
    pair<int, int>* Pair = tableInfo.get(key);
    if (!Pair){
        Pair = new  pair<int, int>(0, num);
        tableInfo.put(key, Pair);
        return 0;
    }
    int first = Pair->first + Pair->second;
    Pair->second += num;
    return first;
}

void DBClient::put(short int userId, short int dataId, short int processId, Data* data){
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

Data* DBClient::get(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    if (!table) return NULL;
    vector<StringWithHash*> keys = table->keys();
    pair<int, int>* info = tableInfo.get(&key);
    vector<QString> headers(keys.size());
    vector<vector<QString> > cells(info->second);
    for (short int i = 0; i < keys.size(); i++){
        QString header = keys[i]->getValue();
        headers[i] = header;
        HashTable<IntWithHash, StringWithHash>* column = table->get(keys[i]);
        for (int j = 0; j < info->second; j++){
            QString cell;
            if (!column){
                cell = "";
                cells[j].push_back(cell);
                continue;
            }
            IntWithHash intKey(info->first + j);
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
    return new Data(headers, cells, info->first);
}

Data* DBClient::get(short int userId, short int dataId, short int processId, int strNum){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || (strNum < info->first) || (strNum >= (info->first + info->second))) return NULL;
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
    return new Data(headers, cells, strNum);
}

Data* DBClient::get(short int userId, short int dataId, short int processId, int strNum, int count){
    TableKey key(userId, dataId, processId);
    HashTable<StringWithHash, HashTable<IntWithHash, StringWithHash> >* table = hashTable.get(&key);
    pair<int, int>* info = tableInfo.get(&key);
    if ((!table) || ((strNum + count) <= info->first) || (strNum >= (info->first + info->second))) return NULL;
    vector<StringWithHash*> keys = table->keys();
    vector<QString> headers(keys.size());
    int firstIndex = (strNum < info->first)? info->first : strNum;
    int size = (((count + strNum) > (info->second + info->first))? (info->second + info->first) : (count + strNum)) - firstIndex;
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
    return new Data(headers, cells, firstIndex);
}

void DBClient::remove(short int userId, short int dataId, short int processId){
    TableKey key(userId, dataId, processId);
    hashTable.remove(&key);
    tableInfo.remove(&key);
}

Data* DBClient::getNextStrings(short int userId, short int dataId, short int processId, short int count){
    TableKey key(userId, dataId, processId);
    IntWithHash* increment = sessions.get(&key);
    if (!increment){
        increment = new IntWithHash(0);
        sessions.put(&key, increment);
    }
    int firstIndex = increment->getValue();
    pair<int, int>* info = tableInfo.get(&key);
    if ((increment->getValue() + count) >= (info->first + info->second)){
        sessions.remove(&key);
    }else{
        increment->setValue(increment->getValue() + count);
    }
    return get(userId, dataId, processId, firstIndex, count);
}
