#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "../../ClientLib/client.h"

class RouterClient : public Client
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, bool isReplyca);
};

RouterClient::RouterClient(){

}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, bool isReplyca = false){
    QByteArray msgReq;
    QByteArray msgResp;

    if (isReplyca) {
        json.insert("isReplyca", true);
    }

    msgReq = createMessage(json);

    socket=new QTcpSocket();
    socket->connectToHost(QHostAddress(address), port);
    socket->write(msgReq);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    int sizeMsg;
    while (socket->bytesAvailable()) {
        sizeMsg = socket->bytesAvailable();
        msgResp = socket->readAll();
    }
    socket->close();
    return parseMessage(msgResp);
}

#endif // ROUTERCLIENT_H
