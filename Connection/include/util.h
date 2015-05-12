#pragma once
#include <string>
#include <sstream>
#include <QDataStream>
#include <QByteArray>

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
    QByteArray pack_(const T& s) {
        QByteArray ar;
        QDataStream ss(&ar, QIODevice::ReadWrite);
        ss << s;
        return ar;
    }
    
    template<typename T1, typename... T> 
    QByteArray pack_(const T1& s, const T&... a) {
        QByteArray ar;
        QDataStream ss(&ar, QIODevice::ReadWrite);
        ss << s;
        ar.append(pack_( a...));
        return ar;
    }
    
    template<typename... T> 
    QByteArray pack(const T&... a) {
        return pack_( a...);        
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
        int get(IdIndex i) {
            switch(i) {
                case userIdIndex: return i0;
                case scriptIdIndex: return i1;
                case dataIdIndex: return i2;
            }
            return i2;
        }
        
        void set(IdIndex i, int newValue) {
            switch(i) {
                case userIdIndex: 
                    i0 = newValue;
                    break;
                case scriptIdIndex: 
                    i1 = newValue;
                    break;
                case dataIdIndex: 
                    i2 = newValue;
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
}