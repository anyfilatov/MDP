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
INST_LOGGGER();
/*
 * Simple C++ Test Suite
 */

const char* uCode1 = "\n"
"function map(data)\n"
"   print(\"in lua map\")\n"
"   print(data)\n"
"   t = {}\n"
"   t[0] = \"aa\"\n"
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
"   res2 = doMap(funName, in1)\n"
"   print(\"start reduce\")\n"
"   res3 = doReduce(\"reduce\", res2)\n"
"end";

const char* uCode = "function map(data)\n"
"outTable = {}\n"
"sum = 0\n"
"for k, v in pairs(data) do\n"
"        if tonumber(k) > 2 then\n"
"            table.insert(outTable, tonumber(v))\n"
"            sum = sum + tonumber(v)\n"
"            end\n"
"            end\n"
"\n"
"            if data[1] < 0 then\n"
"                outTable[\"-1\"] = sum / 12\n"
"            else\n"
"                outTable[\"-2\"] = sum / 12\n"
"                    end\n"
"                    lat_long = tostring(data[1]) ..\"_\" ..tostring(data[2])\n"
"                outTable[lat_long] = sum / 12\n"
"\n"
"                return outTable;\n"
"end\n"
"\n"
"function reduce(key, value)\n"
"sum = 0\n"
"\n"
"for k, v in pairs(value) do\n"
"        sum = sum + tonumber(v)\n"
"        end\n"
"        return key, sum / table.getn(value)\n"
"        end\n"
"\n"
"        function main()\n"
"        funName = \"map\"\n"
"            print(\"start map\")\n"
"        res2 = doMap(funName, in1)\n"
"        print(\"start reduce\")\n"
"        res3 = doReduce(\"reduce\", res2)\n"
"        print(\"start reduce\")\n"
"        end";

void test1() {
    QString ip = "127.0.0.1";
    int port = 4001;
    std::cout << "SendTask test 1" << std::endl;
    int cmd(util::CMD_START_USER_SCRIPT);
    int id = 1;
    
    QByteArray buffer = util::pack(cmd, QString(uCode), id);
    QTcpSocket socket;
    socket.connectToHost(ip, port);
    if( socket.isWritable() ){
        socket.write(buffer);
        QByteArray in;
        int res = util::readAll(socket, in, 1000);
        if(res == Errors::STATUS_OK) {
            QDataStream data(&in, QIODevice::ReadWrite);
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
    Logger::addConsoleLogger();
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

