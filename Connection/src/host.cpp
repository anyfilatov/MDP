#include "host.h"
#include <iostream>
#include <QString>
#include "Logger.h"

Host::Host()
{
    this->IP = "";
    this->port = -1;
}

Host::Host(QString IP, int port)
{
    this->IP = IP;
    this->port = port;
}

Host::~Host(){

}

void Host::setHost(QString IP, int port){
    this->IP = IP;
    this->port = port;
}

QString Host::getIP(){
    return this->IP;
}

int Host::getPort(){
    return this->port;
}

int Host::send(HostContent& )const{
    
    return 0;
}

int Host::run(){
    return 0;
}
