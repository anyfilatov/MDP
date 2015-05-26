#include <QHostAddress>
#include <QDebug>

#include "server.h"

Server::Server()
{
    if (!listen(QHostAddress("127.0.0.1"), 12345)) {
        qDebug() << "Cant bind";
        return;
    }
    qDebug() << "Server startred";
}

void Server::run()
{
    while(true) {
        this->waitForNewConnection(10000);
        if (hasPendingConnections()) {
            socket = this->nextPendingConnection();
            qDebug() << "New connection";

            read();
        }
    }
}

void Server::read()
{
    QDataStream stream(socket);
    socket->waitForReadyRead();
    while(true) {
        if (packetSize == 0) {
            if (socket->bytesAvailable() < (quint32) sizeof(quint32)) {
                qDebug() << "Bytes available " << socket->bytesAvailable();
                continue;
            }
            stream >> packetSize;
        }

        if (socket->bytesAvailable() < packetSize) {
            continue;
        }

        QByteArray recievedMessage;
        stream >> recievedMessage;
        qDebug() << "Server recieved packet with size = " << recievedMessage.size();

        QByteArray packet;
        QDataStream out(&packet, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out << (quint32)0;
        out << recievedMessage;
        out.device()->seek(0);
        out << (quint32)(packet.size() - sizeof(quint32));

        socket->write(packet);
        socket->waitForBytesWritten();
        socket->flush();
        packetSize = 0;
        break;
    }
}



