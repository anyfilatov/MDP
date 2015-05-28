#include <qbytearray.h>

#include "Task.h"
#include "LuaExecutor.h"
#include <QList>
#include <qdebug.h>
#include "dbclient.h"

Task::Task(const HostContent& content, RB rb, DB db, OG og)
: rb_(rb), db_(db), content_(content), og_(og) {
}

Task::~Task(void) {
}

//void Task::readyRead() {
//    
//}



int Task::operator()() {
    QString resString;
    int result = Errors::STATUS_OK;
    auto socket = content_.getSocket();
    try {
        LOG_DEBUG("addSocket");
        auto socket = content_.getSocket();
        QByteArray ar;
//        while(socket->waitForReadyRead(1000)) {
//            ar.append(socket->readAll());
//        }
//        
        if( util::readAll(*socket, ar, 1000) == Errors::STATUS_ERROR){
            LOG_ERROR("Read from socket error");
            socket->close();
            return Errors::STATUS_ERROR;
        }
        QDataStream data(&ar, QIODevice::ReadWrite);
        //if( readAll(data) == Errors::STATUS_OK ) {
        int cmd = 0;
        int id = 0;
        data >> cmd;
        QString code;
        if(cmd != util::CMD_PING && cmd != util::CMD_SET_CONFIG){
            data >> code;
            LOG_DEBUG("cmd=" << cmd );
            if (code.isEmpty()) {
                throw UnknownParametersException(util::concat("Lua code is empty"));
            }
            LOG_DEBUG("code=" + code.toStdString());
            executor_ = std::make_shared<LuaExecutor>(db_, rb_, og_, code, cmd);
        }
        LOG_DEBUG("cmd=" << cmd << " id=" << id);
        switch (cmd) {
            case util::CMD_MAP:
                unpackAndExec(cmd);
                doMap(data);
                break;
            case util::CMD_REDUCE:
                unpackAndExec(cmd);
                doReduce(data);
                break;
            case util::CMD_START_USER_SCRIPT:
                unpackAndExec(cmd);
                doUserScript(data);
                break;
            case util::CMD_PING:
                ping(data);
                break;
            case util::CMD_SET_CONFIG:
                setConfig(data);
                break;
            default:
                throw UnknownParametersException(util::concat("Unknown command type"));
        }
    //} else {
    //    throw ReadFromSocketException("read all error");
    //}
    } catch (const std::exception& e) {
        LOG_ERROR("exception:" << e.what());
        auto s = util::concat("execution script exception:", e.what());
        resString.append(s.c_str());
        result = Errors::STATUS_COMPILATION_ERROR;
        QByteArray arr;
        QDataStream ss(&arr, QIODevice::ReadWrite);
        ss << result << resString;
        socket->write(arr);
    }
    LOG_DEBUG("before flush");
    socket->flush();
    LOG_DEBUG("before wait");
    socket->waitForBytesWritten();
    LOG_DEBUG("before close");
    socket->close();
    LOG_DEBUG("after close");
    socket->deleteLater();
    return 0;
}

int Task::ping(QDataStream&) {
    auto socket = content_.getSocket();
    QByteArray arr = util::pack(Errors::STATUS_OK);
    socket->write(arr);
    return 0;
}

int Task::setConfig(QDataStream& stream) {
    auto socket = content_.getSocket();
    QList<QString> list;
    stream >>  list;
    for(auto& s : list){
        LOG_DEBUG("s=" << s.toStdString());
    }
    
    QByteArray ar = util::pack(Errors::STATUS_OK);
    socket->write(ar);
    return 0;
}

int Task::unpackAndExec(int ) {
    
    return Errors::STATUS_OK;
}

int Task::doMap(QDataStream& stream) {
    auto socket = content_.getSocket();
    util::Id id;
    QString funcName;
    stream >> id >> funcName;
    executor_->setId(id);
    executor_->setFuncName(funcName);
    auto res = executor_->execute();
    
    QByteArray arr = util::pack(res);
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doReduce(QDataStream& stream) {
    auto socket = content_.getSocket();
    util::Id id;
    QString funcName;
    QStringList keys;
    stream >> id >> funcName >> keys;
    LOG_INFO("Recieve keys:" << keys.size());
    executor_->setId(id);
    executor_->setFuncName(funcName);
    executor_->setKeys(keys);
    auto res = executor_->execute();
    QByteArray arr = util::pack(res);
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doUserScript(QDataStream& stream) {
    auto socket = content_.getSocket();
    int id = 0;
    stream >> id;
    executor_->setId(util::Id{id, 0, 0});
    executor_->setFuncName("main");
    auto res = executor_->execute();
    QByteArray arr = util::pack(res);
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doPing(QDataStream&) {
    auto socket = content_.getSocket();
    QByteArray arr = util::pack(Errors::STATUS_PING_OK);
    socket->write(arr);
    return Errors::STATUS_OK;
}
