#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "ClientLib/client.h"

class RouterClient : public Client
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca=false);
private:
    int openConnection(QString host, int port);
};

#endif // ROUTERCLIENT_H
