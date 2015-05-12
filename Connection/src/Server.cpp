/* 
 * File:   Server.cpp
 * Author: savinov
 * 
 * Created on March 25, 2015, 11:22 PM
 */

#include "Server.h"
#include <qstring.h>
#include "hostsender.h"
#include "errors.h"

using namespace Errors;

static const char* CMD_START_USER_SCRIPT = "start";
static const char* CMD_START_MAP = "map";
static const char* CMD_START_REDUCE = "reduce";
static const char* CMD_PING = "ping";
static const char* CMD_END_MAP = "mapEnded";
static const char* CMD_END_REDUCE = "reduceEnded";

Server::Server(QString ip, int port, const DB& db, const RB& rb, OG& og) 
        : Host(ip, port), threadPool_(THREAD_COUNT)
        , tasksMap_(new TaskCounterMap()), rbTree_(rb), dataBase_(db), orGraph_(og) {
}

Server::~Server() {
}

int Server::run(){
    if(configure() == STATUS_OK) {
        threadPool_.start();
        while (tcpServer_.isListening()) {
            LOG_DEBUG("Before wait");
            tcpServer_.waitForNewConnection(WAIT_CONNECTION_TIME_MSEC, nullptr);
            LOG_DEBUG("After wait");
            if (tcpServer_.hasPendingConnections()) {
                LOG_DEBUG("has connection");
                auto socket = tcpServer_.nextPendingConnection();
                if (socket) {
                    HostContent content(Host(), socket);
                    Task task(content, rbTree_, dataBase_, orGraph_);
                    threadPool_.add(task);
//                    LOG_DEBUG("addSocket");
//                    QString data;
//                    auto buff = socket->readAll();
//                    QString str(buff);
//                    LOG_DEBUG("str=" << str.toStdString());
//                    socket->write("Ok", strlen("Ok"));
//                    LOG_DEBUG("before flush" );
//                    socket->flush();
//                    LOG_DEBUG("before wait" );
//                    socket->waitForBytesWritten();
//                    LOG_DEBUG("before close" );
//                    socket->close();
//                    LOG_DEBUG("after close" );
//                    QDataStream stream(socket);
//                    QString cmd;
//                    stream >> cmd;
                }
            }
            LOG_DEBUG("end");
        }
        threadPool_.stop();
        return STATUS_OK;
    }
    return STATUS_ERROR;
}

int Server::send(const HostContent& )const{
//    Task task(data, rbTree_, dataBase_);
//    threadPool_.add(task);
    return STATUS_OK;
}

int Server::configure() {
//    QHostAddress addr(QHostAddress::Any);
    QHostAddress addr(QHostAddress::Broadcast);
    addr.setAddress(IP);
    if( !tcpServer_.listen(addr, qint16(port) ) ) {
        LOG_WARNING("tcpServer listen error. Exit");
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

int Server::unpack(QTcpSocket& socket, QString* data) {
    int status = STATUS_ERROR;
    if( socket.waitForReadyRead() ) {
        QByteArray readedData = socket.readAll();
        if(readedData.isEmpty()) {
            LOG_WARNING("readedData isEmpty");
            status = STATUS_ERROR;
        } else {
            data->append(readedData);
            status = STATUS_OK;
        }
    }
    return status;
}

void Server::sendOk(QTcpSocket& socket) {
    socket.putChar('0');
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
    socket.deleteLater();
    
}

void Server::sendError(QTcpSocket& socket) {
    socket.putChar('1');
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
    socket.deleteLater();
}

