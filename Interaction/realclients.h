#pragma once
#include "abstractClient.h"
#include "client/client.h"
#include "clientDisp/ExecutionClient.h"
#include "DispatcherData/dispatcherclient.h"
#include <QTextEdit>
#include <QObject>

namespace integration {
    class RealClientBD : public DispatcherClient, public AbstractClient {
        Q_OBJECT
    private:
        bool connected = false;
    public:
        RealClientBD(const QString& strHost, int nPort) : DispatcherClient(strHost, nPort,strHost,nPort) {}
        bool CheckHosts(IP &, std::vector<QString> &);
        void SendConfig(IP &, std::vector<QString>&);
        void UpdateConfig();
        QString getName(){return "Client of DataBase";}
    signals:
        void logStr(QString);
    };

    class RealClientRBT : public Client, public AbstractClient {
        Q_OBJECT
    public:
        RealClientRBT(QString strfile):Client(strfile){}
        bool CheckHosts(IP &, std::vector<QString>&);
        void SendConfig(IP &, std::vector<QString>&);
        void UpdateConfig();
        QString getName(){return "Client of Red-Black Tree";}
    signals:
        void logStr(QString);
    };

    class RealClientDisp : public QObject, public AbstractClient, public Execution::Client{
        Q_OBJECT
    public:
        RealClientDisp(QString iip, qint32 port) : Execution::Client(iip,port){}
        bool CheckHosts(IP &, std::vector<QString>&);
        void SendConfig(IP &, std::vector<QString>&);
        void UpdateConfig();
        QString getName(){return "Client of Dispatcher";}
    signals:
        void logStr(QString);
    };

    /*std::ostream& operator << (std::ostream&, RealClientBD&);
    std::ostream& operator << (std::ostream&, RealClientRBT&);
    std::ostream& operator << (std::ostream&, RealClientDisp&);*/
}

