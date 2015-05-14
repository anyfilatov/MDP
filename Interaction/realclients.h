#pragma once
#include "abstractClient.h"
#include "client/client.h"
#include "clientDisp/ExecutionClient.h"
#include "DispatcherData/dispatcherclient.h"

namespace integration {
    class RealClientBD : public AbstractClient, public DispatcherClient {
    public:
        RealClientBD(const QString& strHost, int nPort) : DispatcherClient(strHost, nPort) {}
        bool CheckHosts(IP &);
        void SendConfig(IP &);
        void UpdateConfig();
    };

    class RealClientRBT : public AbstractClient , public Client{
    public:
        RealClientRBT(QString strfile):Client(strfile){}
        bool CheckHosts(IP &);
        void SendConfig(IP &);
        void UpdateConfig();
    };

    class RealClientDisp : public AbstractClient, public Execution::Client{
    public:
        RealClientDisp(QString iip, qint32 port) : Execution::Client(iip,port){}
        bool CheckHosts(IP &);
        void SendConfig(IP &);
        void UpdateConfig();
    };
}

