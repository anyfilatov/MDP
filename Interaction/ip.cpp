#include "ip.h"
using namespace integration;

QString IP::GetIP(){
    return ip;
}

qint32 IP::GetPort(){
    return port;
}

QString IP::GetFullAddress(){
    QString res = "";
    res.sprintf("%s:%d",ip.toStdString().c_str(),port);
    return res;
}

void IP::SetFullAddress(const QString &ip, const qint32 &port){
    this->ip = ip;
    this->port = port;
}
