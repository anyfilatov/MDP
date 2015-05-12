#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "../../ClientLib/client.h"

class RouterClient : public Client
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca=false);
private:
    int openConnection(QString host, int port);
};

RouterClient::RouterClient(){

}

int RouterClient::openConnection(QString host, int port){
    if (_socket == NULL) {
        _socket=new QTcpSocket();
        connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }

    if(_socket->state() == QAbstractSocket::UnconnectedState){
        _socket->connectToHost(QHostAddress(host), port);

        if (!_socket->waitForConnected(1000)){
            qDebug("Can't connect");
            return -1;
        }
    }
    return 0;
}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca){
    openConnection(address, port);
    QJsonObject jsonResp;

    if (isReplyca) {
        json.insert("isReplyca", true);
    }

    json.insert("not_forwards_requests", true);

    writeMsg(json);
    jsonResp = readMsg();


    return jsonResp;
}

#endif // ROUTERCLIENT_H
