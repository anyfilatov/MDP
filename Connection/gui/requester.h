#ifndef REQUESTER
#define REQUESTER

#include "util.h"
#include "Logger.h"
#include <QByteArray>
#include <QTcpSocket>
#include <thread>
#include <condition_variable>
#include <atomic>
#include "errors.h"

template<typename T>
class AsyncRequester{
    QString ip_;
    int port_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    QString code_;
    int id_ = 0;
    T onRecieveAnswer_;
    std::atomic_bool stop_ ;
    class Caller{
        AsyncRequester<T>* req_;
    public:
        Caller(AsyncRequester<T>* req) : req_(req){}
        Caller(const Caller&) = default;
        void operator()(){
            (*req_)();
        }
    };
public:

    AsyncRequester(QString ip, int port) : ip_(ip), port_(port), thread_(Caller(this)),stop_(false){}
    AsyncRequester(const AsyncRequester&) = delete;
    ~AsyncRequester(){
        stop_ = true;
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.notify_all();
        thread_.join();
    }
    void request(QString uCode, int id, T onRecieveAnswer ) {
        onRecieveAnswer_ = onRecieveAnswer ;
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.notify_all();
        code_ = uCode;
        id_ = id;
    }

    int operator ()(){
        while(!stop_){
            try{
                std::unique_lock<std::mutex> lock(mutex_);
                cond_.wait(lock );
                int cmd(util::CMD_START_USER_SCRIPT);

                QByteArray buffer = util::pack(cmd, code_, id_);
                QTcpSocket socket;
                socket.connectToHost(ip_, port_);
                if( socket.isWritable() && socket.state() != QTcpSocket::UnconnectedState){
                    socket.write(buffer);
                    QByteArray in;
                    int res = util::readAll(socket, in, 10000);
                    if(res == Errors::STATUS_OK) {
                        QDataStream data(&in, QIODevice::ReadWrite);
                        int res = -1;
                        QString s;
                        data >> res;
                        data >> s;
                        onRecieveAnswer_(s, res);
                        LOG_INFO("res=" << res << " strRes=" + s.toStdString());
                    } else {
                        throw NetworkErrorException("Error: no answere from server");
                    }

                } else {
                    throw NetworkErrorException("Error: server unavailble");
                }
                code_.clear();
            } catch(const std::exception& ex){
                auto str = util::concat( "Host(", ip_.toStdString(), ":", port_, ")", ex.what());
                onRecieveAnswer_(QString(str.c_str()), -1);
                code_.clear();
            }
        }
    }
};
#endif // REQUESTER

