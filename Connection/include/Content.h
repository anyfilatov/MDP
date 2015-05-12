#pragma once
#include <QString>
#include <QTcpSocket>
#include "Logger.h"

template<typename THost>
class Content {
public:
    Content(const THost& host, QTcpSocket*  socket) : host_(host), socket_(socket){}
    QString getData() const {
        return data_;
    }
    void setData(const QString& data) {
        data_ = data;
    }
    THost& getHost(){
        return host_;
    }
    int send() {
        LOG_WARNING("Not implemented");
        return 0;
    }
    
    QTcpSocket* getSocket() {
        return socket_;
    }
private:
    QString data_;
    THost host_;
    QTcpSocket* socket_;
};
