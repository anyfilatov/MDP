#ifndef CLIENT_H
#define CLIENT_H

#include <QFile>
#include <QJsonArray>
#include "ClientLib/StatusCodes.h"
#include "ClientLib/typerequest.h"
#include "abstractclient.h"
#include "Exception/exception.h"

class Client: public AbstractClient
{
public:
    Client(int maxBufferSize, QString settingsFileName);

    void put(const QString& key, const QString& value);
    void put(const QString& key, const QString& value, const QString& bucket);

    QStringList get(const QString& key);
    QStringList get(const QString& key, const QString& bucket);

    QStringList getBucketKeys(const QString& bucket);

    void remove(const QString& key);
    void remove(const QString& key, const QString& bucket);

    QStringList getRingHosts();
    void joinToRing(const QString& who, const QStringList& ring);

    void flush();
    bool connectToHost();

private:
    void putRewriteValue(const QString& key, const QString& value);
    void removeValue(const QString& key, const QString& value);

    const int MAX_BUFFER_SIZE;
    QJsonArray _buffer;
    QStringList _hosts;
};

#endif // CLIENT_H
