/* 
 * File:   SendTask.cpp
 * Author: savinov
 *
 * Created on May 10, 2015, 8:35:04 PM
 */

#include <stdlib.h>
#include <iostream>
#include "util.h"
#include "Logger.h"
#include <QByteArray>
#include <QTcpSocket>

/*
 * Simple C++ Test Suite
 */

const char* uCode = "\n"
"function map(data)\n"
"   print(\"in lua map\")\n"
"   t = {}\n"
"   t[0] = data\n"
"   t[1] = \"1\"\n"
"   t[2] = \"aaaa\"\n"
"   t[3] = \"2\"\n"
"   return t\n"
"end\n"
"\n"
"function reduce(key, value)\n"
"    print(key)\n"
"    for k, v in pairs(value) do \n"
"       print(k, v) \n"
"    end\n"
"    return key, \"1\"\n"
"end\n"
"\n"
"function main()\n"
"   a = \"test\"\n"
"   print(a)\n"
"   funName = \"map\"\n"
"   res2 = doMap(funName, in0)\n"
"   print(\"start reduce\")\n"
"   res3 = doReduce(\"reduce\", res2)\n"
"end" ;

void test1() {
    QString ip = "127.0.0.1";
    int port = 4001;
    std::cout << "SendTask test 1" << std::endl;
    int cmd(util::CMD_START_USER_SCRIPT);
    int id = 1;
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::ReadWrite);
    stream << cmd;
    QString s(uCode);
    stream << s;
    stream << id;
    QTcpSocket socket;
    socket.connectToHost(ip, port);
    if( socket.isWritable() ){
        socket.write(buffer);
        bool res= socket.waitForReadyRead();
        while( !res ) {
            if(!socket.isOpen()){
                break;
            }
            res= socket.waitForReadyRead();
        }
        if(res) {
            QDataStream data(&socket);
            int res = -1;
            QString s;
            data >> res >> s;
            LOG_DEBUG("res=" << res << " strRes=" + s.toStdString());
        } else {
            LOG_ERROR("res = false");
        }
        
    } else {
        LOG_DEBUG("can't write");
    }
}

void test2() {
    std::cout << "SendTask test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (SendTask) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
//    std::cout << "%SUITE_STARTING% SendTask" << std::endl;
//    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (SendTask)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (SendTask)" << std::endl;
//
//    std::cout << "%TEST_STARTED% test2 (SendTask)\n" << std::endl;
//    test2();
//    std::cout << "%TEST_FINISHED% time=0 test2 (SendTask)" << std::endl;
//
//    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

