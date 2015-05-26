#ifndef REMOTECLIENT_H
#define REMOTECLIENT_H

#include <QTcpSocket>
#include <QSettings>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHostAddress>
#include "Router/typerequest.h"
#include "Router/StatusCodes.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(int maxBufferSize,QObject *parent = 0);
    explicit Client(int maxBufferSize, QString settingsFileName,QObject *parent = 0);
    ~Client();

    int put(QString key, QString value, QString bucket = NULL);
    QStringList get(QString key, QString bucket = NULL);
    QStringList getBucketKeys(QString bucket);
    int remove(QString key, QString bucket = NULL);
    int removeOne(QString key, QString value, QString bucket = NULL);
    QStringList getRingHosts();
    void joinToRing(QString who, QStringList ring);

    void flush();
    bool isConnected();
    void disconnectFromHost();

signals:

public slots:

protected:
    QTcpSocket* _socket = NULL;
    QJsonArray _buffer;
    const int MAX_BUFFER_SIZE;
    const int WAIT_TIME_MLS;
    QStringList _hosts;

    void openConnection();
    void write(const QJsonObject &packet);
    QJsonObject read();
};

#endif // REMOTECLIENT_H
