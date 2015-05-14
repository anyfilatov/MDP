#include "routerclient.h"

RouterClient::RouterClient(){

}

int RouterClient::openConnection(QString host, int port){
    qDebug() << "Conected to " << host << ":" << port;
    if (_socket == NULL) {
        _socket=new QTcpSocket();
        connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }

    if(_socket->state() == QAbstractSocket::ConnectedState){
        _socket->disconnect();
    }

    _socket->connectToHost(QHostAddress(host), port);

    if (!_socket->waitForConnected(1000)){
        qDebug("Can't connect");
        return -1;
    }

    return 0;
}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca){
    qDebug() << "\tdoRequestToOtherRouter: " << address << ":" << port;

//    openConnection(address, port);
    _socket = new QTcpSocket();
    _socket->connectToHost(QHostAddress(address), port);
    QJsonObject jsonResp;

    if (isReplyca) {
        json.insert("isReplyca", true);
    }

    json.insert("not_forwards_requests", true);
    qDebug() << "\treq doRequestToOtherRouter: " << json;
    writeMsg(json);
    jsonResp = readMsg();

    qDebug() << "\tresp doRequestToOtherRouter: " << jsonResp;
    _socket->close();
    delete _socket;
    return jsonResp;
}
