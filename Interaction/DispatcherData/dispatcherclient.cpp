#ifndef DISPATCHERCLIENT_CPP
#define DISPATCHERCLIENT_CPP
#include <QString>
#include "dispatcherclient.h"

DispatcherClient::DispatcherClient(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    tryNum = 0;
    ip = strHost;
    spareIp = strSpareHost;
    port = nPort;
    sparePort = nSparePort;
    //_connect();

}



void DispatcherClient::_connect(){
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(ip, port);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    //connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    qDebug() << "DBClient: Started; server host: " << ip << ", port: " << port;
}

void DispatcherClient::switchIps(){
    QString str = ip;
    ip = spareIp;
    spareIp = str;
    int n = port;
    port = sparePort;
    sparePort = n;
}

void DispatcherClient::setIps(const QString &strHost, int nPort, const QString &strSpareHost, int nSparePort){
    ip = strHost;
    spareIp = strSpareHost;
    port = nPort;
    sparePort = nSparePort;
    _connect();
}

QJsonObject DispatcherClient::slotReadyRead(QJsonObject &obj)
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        //bool l = m_pTcpSocket->
        if (m_pTcpSocket->waitForReadyRead(2000)){
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
            if (tryNum > 4){
                QJsonObject jso;
                jso.insert("TIME_OUT", true);
                return jso;
            }
            m_pTcpSocket->close();
            delete m_pTcpSocket;
            switchIps();
            _connect();
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

void DispatcherClient::slotError(QAbstractSocket::SocketError err)
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
    qDebug() << "DispatcherClient: ERROR: " << strError;
}

void DispatcherClient::sendToServer(const QJsonObject& jso)
{
    QJsonDocument jsDoc(jso);

    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) << jsDoc.toJson();

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    m_pTcpSocket->write(arrBlock);
    qDebug() << "DispatcherClient: sended to server -> " << jsDoc.toJson();
}

void DispatcherClient::slotConnected()
{
    qDebug() << "DispatcherClient: Connect success";
}

//end of

std::vector<QString> DispatcherClient::pingAll()
{
    QJsonObject obj;
    obj.insert("COMMAND", "PING_ALL");
    sendToServer(obj);
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        throw QString("DBClient: Request time out!");
    }
    QJsonArray arr = obj2.take("HOSTS").toArray();
    std::vector<QString> s;
	for (int i=0; i<arr.size(); i++){
        s.push_back(arr[i].toString());
	}
    return s;
}

void DispatcherClient::addHost(const QString& ip, int port)
{
    QJsonObject obj;
    obj.insert("COMMAND", "ADD_HOST");
    obj.insert("IP", ip);
    obj.insert("PORT", port);
    sendToServer(obj);
    QJsonObject obj2 = slotReadyRead(obj);
    if (obj2.contains("TIME_OUT")){
        throw "DBClient: Request time out!";
    }
}

#endif
