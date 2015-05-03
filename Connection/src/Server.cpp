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

Server::Server(QString ip, int port) : Host(ip, port), threadPool_(THREAD_COUNT), orGraph_(*this), tasksMap_(new TaskCounterMap()), rbTree_(new RbTree()), dataBase_(new DataBase()) {
}

Server::~Server() {
}

int Server::Run(){
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
                    LOG_DEBUG("addSocket");
                    QString data;
                    if( unpack(*socket, &data) == STATUS_OK ) {
                        Host host(socket);
                        orGraph_.sendAllChidren(HostSender(HostContent(data, host)));
                    }
                }
            }
            LOG_DEBUG("end");
        }
        threadPool_.stop();
        return STATUS_OK;
    }
    return STATUS_ERROR;
}

int Server::send(const HostContent& data)const{
    Task task(data, rbTree_, dataBase_);
    threadPool_.add(task);
    return STATUS_OK;
}

int Server::configure() {
    QHostAddress addr(QHostAddress::Any);
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

