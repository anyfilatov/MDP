#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "ClientLib/abstractclient.h"

class RouterClient: public AbstractClient
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, int port);
};

#endif // ROUTERCLIENT_H
