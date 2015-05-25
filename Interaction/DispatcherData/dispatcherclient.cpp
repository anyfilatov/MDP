#ifndef DISPATCHERCLIENT_CPP
#define DISPATCHERCLIENT_CPP
#include <QString>
#include "dispatcherclient.h"

DispatcherClient::DispatcherClient(const QString& strHost, int nPort, QObject *parent) :
    QObject(parent), m_nNextBlockSize(0)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    //connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    qDebug() << "DispatcherClient: Started; server host: " << strHost << ", port: " << nPort;
}

QJsonObject DispatcherClient::slotReadyRead()
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
    qDebug() << "DispatcherClient: received array from server: " << arr;
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
    QJsonObject receivedObj = slotReadyRead();
    QJsonArray arr = receivedObj.take("HOSTS").toArray();
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
}

#endif
