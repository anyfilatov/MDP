#include "client.h"
#include <QHostAddress>
#include <QPair>

Client::Client(QString host, int port, QObject *parent)
    : QObject(parent)
{
    socket=new QTcpSocket(this);
    this->host = host;
    this->port = port;
}

void Client::put(QString key, QString value) {
    socket->connectToHost(QHostAddress(host), port);
    if (!socket->waitForConnected(1000))
        qDebug("Can't connect");
    else {
        QByteArray data;

        data.append(49);
        data.append(48);
        data.append(key);
        data.append('#');
        data.append(value);
        data.resize(1026);

        socket->write(data);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        while (socket->bytesAvailable()) {
            qDebug() << "Reading: " << socket->bytesAvailable();
            qDebug() << socket->readAll();
        }
        socket->close();
    }
}

QList<QString> Client::get(QString key) {
    socket->connectToHost(QHostAddress(host), port);
    QList<QString> response;
    if (!socket->waitForConnected(1000)) {
        qDebug("Can't connect");
        return response;
    }
    else {

        QByteArray data;
        data.append(50);
        data.append(48);
        data.append(key);
        data.resize(1026);

        socket->write(data);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        QString *resp;
        while (socket->bytesAvailable()) {
            resp = new QString(socket->readAll());
        }
        socket->close();
        response = resp->split('#');
        return response;
    }
}


int Client::remove(QString key) {
    socket->connectToHost(QHostAddress(host), port);
    int response = 0;
    if (!socket->waitForConnected(1000)) {
        qDebug("Can't connect");
        return 0;
    }
    else {

        QByteArray data;
        data.append(51);
        data.append(48);
        data.append(key);
        data.resize(1026);

        socket->write(data);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        QByteArray resp;
        while (socket->bytesAvailable()) {
            resp = socket->readAll();
            response = (static_cast<unsigned int>(resp[0]) & 0xFF) << 8
                                                                        + (static_cast<unsigned int>(resp[1]) & 0xFF);
        }
        socket->close();
        return response;
    }
}

QList<QPair<QString, QList<QString> > > Client::getAll() {
    socket->connectToHost(QHostAddress(host), port);
    QList<QPair<QString, QList<QString> > > response;
    if (!socket->waitForConnected(1000)) {
        qDebug("Can't connect");
        return response;
    }
    else {

        QByteArray data;
        data.append(52);
        data.append(48);
        data.resize(1026);

        socket->write(data);
        socket->waitForBytesWritten();

        socket->waitForReadyRead();
        QString *resp;
        while (socket->bytesAvailable()) {
            resp = new QString(socket->readAll());
        }
        socket->close();
        QString entry;
        foreach (entry, resp->split('|')) {
            QList<QString> pair = entry.split(':');
            QString key = pair.first();
            QList<QString> values = (pair.last()).split('#');
            response.append(QPair<QString, QList<QString> >(key, values));
        }
        return response;
    }
}
