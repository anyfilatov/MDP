#ifndef ROUTERCLIENT_H
#define ROUTERCLIENT_H

#include "ClientLib/remoteclient.h"

class RouterClient : public RemoteClient
{
public:
    RouterClient();
    QJsonObject doRequestToOtherRouter(QJsonObject json, QString address, int port, bool isReplyca=false);
private:
    int openConnection(QString host, int port);
};

#endif // ROUTERCLIENT_H
