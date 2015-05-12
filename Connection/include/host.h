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
    void setHost(QString, int);
    QString getIP();
    int getPort();
    virtual int run();
    virtual int send (HostContent& command)const;
    virtual ~Host();
    void setId(int _id) {
        id = _id;
    }
    int getId() {
        return id;
    }
protected:
    QString IP;
    int port;
    int id;
};

