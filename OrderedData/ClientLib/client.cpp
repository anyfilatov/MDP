#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->port = 12340;
    QList<QString> ipsHost;
    ipsHost.append("192.168.1.10");
    this->hosts = ipsHost;
    qsrand (QDateTime::currentMSecsSinceEpoch());
}

int Client::openConnect(){
    if (socket == NULL) {
        socket=new QTcpSocket();
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }

    if(socket->state() == QAbstractSocket::UnconnectedState){
        QString ip = hosts.at(qrand() % hosts.size());
        qDebug() << "Open connect on " << ip;
        socket->connectToHost(QHostAddress(ip), port);

        if (!socket->waitForConnected(1000)){
            qDebug("Can't connect");
            return -1;
        }
    }
    return 0;
}

void Client::writeMsg(QJsonObject json){
    QByteArray data = serialization(json);
    QByteArray message;
    message.append(data.size()+1);
    message.append(data);

    socket->write(message);
    socket->waitForBytesWritten();
}

QJsonObject Client::readMsg(){
    int sizeMsg;
    QByteArray msg;
    socket->waitForReadyRead();
    while (socket->bytesAvailable()) {
        sizeMsg = socket->bytesAvailable();
        msg = socket->readAll();
    }
    return deserialization(msg.mid(1));
}


QJsonObject Client::deserialization(QByteArray data){
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(data);
    return jdoc.object();
}

QByteArray Client::serialization(QJsonObject json){
    QJsonDocument jdoc(json);
    return jdoc.toJson();
}

int Client::remove(QString bucket){
    if (openConnect() == -1){
        return 500;
    }


    QJsonObject jsonReq;
    QJsonObject jsonResp;

    jsonReq.insert("type", DEL);\
    jsonReq.insert("bucket", bucket);

    writeMsg(jsonReq);
    jsonResp = readMsg();

    return jsonResp.value("status").toInt();
}

QList<QString> Client::getRingHosts(){
    if (socket == NULL) {
        openConnect();
    }

    QJsonObject jsonReq;
    QJsonObject jsonResp;

    jsonReq.insert("type", RINGCECK);

    writeMsg(jsonReq);
    jsonResp = readMsg();

    return jsonResp.value("hosts").toString().split(',');
}

void Client::initialOuterJoinRequest(QString idAddress){
    if (socket == NULL) {
        openConnect();
    }

    QJsonObject jsonReq;
    QJsonObject jsonResp;


    jsonReq.insert("type", OUTERJOIN);
    jsonReq.insert("address", idAddress);

    writeMsg(jsonReq);
    jsonResp = readMsg();
}

void Client::disconnected(){
    qDebug() << "Server disconnected";
    qDebug() << socket->state();
}
