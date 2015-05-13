#pragma once
#include <QString>
#include <QByteArray>
#include <QTcpSocket>
#include <QList>

namespace Execution{
    class Client{
    public:
        Client(const QString& ip, int port) : ip_(ip), port_(port){   
            
        }
        void setIp(const QString& ip) {
            ip_ = ip;
        }
        QString getIp() const {
            return ip_;
        }
        
        void setCode(int port) {
            port_ = port;
        }
        int getPort() const {
            return port_;
        }
        
        std::vector<QString> ping() const {
            std::vector<QString> out;
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::ReadWrite);
            stream << int(0) << 4;
            stream.device()->seek(0);
            stream << int(buffer.size() - sizeof(int));
            if( request(buffer) == 0){
                QDataStream streamAnswer(&buffer, QIODevice::ReadWrite);
                int code = 0;
                streamAnswer >> code;
                if(code != 0) {
                    QList<QString> list;
                    QString str;
                    streamAnswer >> list;
                    for(auto& s : list) {
                        out.push_back(s);
                    }
                }
            }
            return out;
        }
        
        void setNewChild(const std::vector<QString>& conf, const std::vector<QString>& newConf) {
            QByteArray buffer;
            QDataStream stream(&buffer, QIODevice::ReadWrite);
            stream << 5;
            QList<QString> lConf;
            for(auto& s : conf) {
                lConf.push_back(s);
            }
            QList<QString> lNewConf;
            for(auto& s : newConf) {
                lNewConf.push_back(s);
            }
            stream << int(0) << lConf << lNewConf;
            stream.device()->seek(0);
            stream << int(buffer.size() - sizeof(int));
            request(buffer);
        }
        
    private:
        int request( QByteArray& inOutBuffer) const {
            QTcpSocket socket;
            socket.connectToHost(ip_, port_);
            if (socket.isWritable()) {
                socket.write(inOutBuffer);
                bool res = socket.waitForReadyRead();
                while (!res) {
                    if (!socket.isOpen()) {
                        break;
                    }
                    res = socket.waitForReadyRead();
                }
                if (res) {
                    inOutBuffer = socket.readAll();
                    return 0;
                }
            }
            return 2;
        }
        
    private:
        QString ip_;
        int port_;
};
}//namespace