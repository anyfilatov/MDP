#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "../../ClientLib/client.h"

class RouterClient : public Client
{
public:
    RouterClient();
    template <typename K, typename V> void put(K key, V value, QString bucket = NULL);
    template <typename K, typename V> QList<V> get(K key);
    int virtual remove(QString bucket);

private:
    QString connectToServer(QString ipAddress);
};

#endif // ROUTERCLIENT_H
