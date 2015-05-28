#pragma once
#include <string>
#include <sstream>
#include <QDataStream>
#include <QByteArray>
#include <QTcpSocket>
#include "Logger.h"
#include "errors.h"

namespace util{
    
    static const int CMD_START_USER_SCRIPT = 1;
    static const int CMD_MAP = 2;
    static const int CMD_REDUCE = 3;
    static const int CMD_PING = 4;
    static const int CMD_SET_CONFIG = 5;
//    static const int CMD_START_USER_SCRIPT = 1;
//    static const int CMD_START_USER_SCRIPT = 1;
    
    template<typename T>
    std::string concat_(const T& s) {
        std::stringstream ss;
        ss << s;
        return ss.str();
    }
    
    template<typename T1, typename... T> 
    std::string concat_(const T1& s, const T&... a) {
        std::stringstream ss;
        ss << s << concat_(a...);
        return ss.str();
    }
    
    template<typename T>
    void pack_(QDataStream& ss, const T& s) {
        ss << s;
    }
    
    template<typename T1, typename... T> 
    void pack_(QDataStream& stream, const T1& s, const T&... a) {
        stream << s;
        pack_(stream,  a...);
    }
    
    template<typename... T> 
    QByteArray pack(const T&... a) {
        QByteArray out;
        QDataStream stream(&out, QIODevice::ReadWrite);
        stream << int(0);
        pack_(stream, a...);
        stream.device()->seek(0);
        stream << (int)(out.size() - sizeof(int));
        return out;
    }
    
    template<typename... T> 
    std::string concat(const T&... a) {
        return concat_(a...);
    }
    
    class Id {
    public:
        Id() = default;
        Id(const Id& ) = default;
        Id( Id&& ) = default;
        Id& operator = (const Id& ) = default;
        Id& operator = ( Id&& ) = default;
        enum IdIndex{
            userIdIndex,
            scriptIdIndex,
            dataIdIndex
        };
        int get(IdIndex i) const {
            switch(i) {
                case userIdIndex: return i0;
                case scriptIdIndex: return i2;
                case dataIdIndex: return i1;
            }
            return i2;
        }
        
        void atoi(const QString& in) {
            QStringList list = in.split(" ");
            if(list.size() < 3) {
                LOG_ERROR("atoi error:" << in.toStdString());
                return;
            }
            auto it = list.begin();
            i0 = it->toInt();
            i1 = it->toInt();
            i2 = it->toInt();
        }

        QString str() const {
            return QString::number(i0) + " " + QString::number(i1) + " " + QString::number(i2);
        }

        void set(IdIndex i, int newValue) {
            switch(i) {
                case userIdIndex: 
                    i0 = newValue;
                    break;
                case scriptIdIndex: 
                    i2 = newValue;
                    break;
                case dataIdIndex: 
                    i1 = newValue;
                    break;
            }
        }
        friend QDataStream& operator << (QDataStream& stream, const Id& id) {
            stream << id.i0 << id.i1 << id.i2;
            return stream;
        }
        
        friend QDataStream& operator >> (QDataStream& stream, Id& id) {
            stream >> id.i0 >> id.i1 >> id.i2;
            return stream;
        }
        
        int i0;
        int i1;
        int i2;
    };
    
    static int readAll(QTcpSocket& socket, QByteArray& out, int timeout) {
        
        while (socket.bytesAvailable() < qint64(sizeof(int))) {
            if(socket.state() == QTcpSocket::UnconnectedState){
                throw RemoteServerNoAnswereException("Remote server close connection");
            }
            socket.waitForReadyRead();
        }
        QDataStream in(&socket);
        
        int dataSize = 0;
        in.setVersion(QDataStream::Qt_5_2);
        in >> dataSize;
        while (out.size() < dataSize) {
            out.append(socket.readAll());
            if (out.size() < dataSize) {
                if (!socket.waitForReadyRead(timeout)) {
                    LOG_DEBUG("4");
                    if(out.size() == 0){
                        return Errors::STATUS_ERROR;
                    } else {
                        break;
                    }
                }
            }
        }
        return Errors::STATUS_OK;
    };
}
