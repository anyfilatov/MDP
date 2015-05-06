#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "../../ClientLib/client.h"

class RouterClient : public Client
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca);
private:
    int openConnect(QString host, int port);
};

RouterClient::RouterClient(){

}

int RouterClient::openConnect(QString host, int port){
    if (socket == NULL) {
        socket=new QTcpSocket();
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }

    if(socket->state() == QAbstractSocket::UnconnectedState){
        socket->connectToHost(QHostAddress(host), port);

        if (!socket->waitForConnected(1000)){
            qDebug("Can't connect");
            return -1;
        }
    }
    return 0;
}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca = false){
    openConnect(address, port);
    QByteArray msgReq;
    QByteArray msgResp;

    if (isReplyca) {
        json.insert("isReplyca", true);
    }

    msgReq = serialization(json);

    socket->write(msgReq);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    int sizeMsg;
    while (socket->bytesAvailable()) {
        sizeMsg = socket->bytesAvailable();
        msgResp = socket->readAll();
    }
    socket->close();
    return deserialization(msgResp.mid(1));
}

#endif // ROUTERCLIENT_H
