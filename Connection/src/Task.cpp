#include <qt5/QtCore/qbytearray.h>

#include "Task.h"
#include "LuaExecutor.h"
#include <QList>
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
        QDataStream data(socket);
        int cmd = 0;
        int id = 0;
        data >> cmd;
        LOG_DEBUG("cmd=" << cmd << " id=" << id);
        switch (cmd) {
            case util::CMD_MAP:
                unpackAndExec(cmd);
                doMap();
                break;
            case util::CMD_REDUCE:
                unpackAndExec(cmd);
                doReduce();
                break;
            case util::CMD_START_USER_SCRIPT:
                unpackAndExec(cmd);
                doUserScript();
                break;
            case util::CMD_PING:
                ping();
                break;
            case util::CMD_SET_CONFIG:
                setConfig();
                break;
            default:
                throw UnknownParametersException(util::concat("Unknown command type"));
        }
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

int Task::ping() {
    auto socket = content_.getSocket();
    QByteArray arr;
    QDataStream s(&arr, QIODevice::ReadWrite);
    s << 0;
    socket->write(arr);
    return 0;
}

int Task::setConfig() {
    auto socket = content_.getSocket();
    QDataStream stream(socket);
    QList<QString> list;
    stream >>  list;
    for(auto& s : list){
        LOG_DEBUG("s=" << s.toStdString());
    }
    QByteArray ar;
    QDataStream streamOut(&ar, QIODevice::ReadWrite);
    streamOut << 0;
    socket->write(ar);
    return 0;
}

int Task::unpackAndExec(int type) {
    auto socket = content_.getSocket();
    QDataStream data(socket);
    QString code;
    data >> code;
    if(code.isEmpty()){
        throw UnknownParametersException(util::concat("Lua code is empty"));
    }
    LOG_DEBUG("code=" + code.toStdString());
    executor_ = std::make_shared<LuaExecutor>(db_, rb_, og_, code, type);
    return Errors::STATUS_OK;
}

int Task::doMap() {
    auto socket = content_.getSocket();
    QDataStream stream(socket);
    util::Id id;
    QString funcName;
    stream >> id >> funcName;
    executor_->setId(id);
    executor_->setFuncName(funcName);
    auto res = executor_->execute();
    
    QByteArray arr;
    QDataStream s(&arr, QIODevice::ReadWrite);
    s << res;
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doReduce() {
    auto socket = content_.getSocket();
    QDataStream stream(socket);
    util::Id id;
    QString funcName;
    stream >> id >> funcName;
    executor_->setId(id);
    executor_->setFuncName(funcName);
    auto res = executor_->execute();
    
    QByteArray arr;
    QDataStream s(&arr, QIODevice::ReadWrite);
    s << res;
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doUserScript() {
    auto socket = content_.getSocket();
    QDataStream data(socket);
    int id = 0;
    data >> id;
    executor_->setId(util::Id{id, 0, 0});
    executor_->setFuncName("main");
    auto res = executor_->execute();
    QByteArray arr;
    QDataStream s(&arr, QIODevice::ReadWrite);
    s << res;
    socket->write(arr);
    return Errors::STATUS_OK;
}

int Task::doPing() {
    auto socket = content_.getSocket();
    QByteArray arr;
    QDataStream s(&arr, QIODevice::ReadWrite);
    s << Errors::STATUS_PING_OK;
    socket->write(arr);
    return Errors::STATUS_OK;
}
