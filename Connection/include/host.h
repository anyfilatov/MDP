#pragma once
#include <QString>
#include <QTcpSocket>
#include "Content.h"
class Host;
typedef Content<Host> HostContent;

class Host
{
public:
    Host();
    Host(QString, int);
    Host(QTcpSocket* socket);
    void setHost(QString, int);
    QString getIP();
    int getPort();
    void run();
    virtual int send (HostContent& command)const;
    virtual ~Host();
protected:
    QString IP;
    int port;
    QTcpSocket* socket_;
};

