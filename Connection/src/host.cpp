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

Host::Host(QTcpSocket* socket) : socket_(socket) {
    this->IP = "";
    this->port = -1;
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
    if(socket_){
        std::string str("alalala");
        LOG_DEBUG("write");
        if( socket_->isWritable() ) {
            //auto res = 
            socket_->write(str.c_str(), str.size());
            LOG_DEBUG("writed:");
            socket_->flush();
            socket_->waitForBytesWritten(-1);
        } else {
            LOG_DEBUG("not writable");
        }
        socket_->close();
        socket_->deleteLater();
    } else {
        //LOG_DEBUG("Host::send:" << c.getHost().getIp().toStdString() << ":" << c.getHost().getPort());
    }
    LOG_DEBUG("send end");
    return 0;
}

void Host::run(){

}
