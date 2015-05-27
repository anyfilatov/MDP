#include "realclients.h"
#include "QFile"
#include <iostream>
using namespace integration;
using std::vector;
using std::pair;

vector<pair<QString,qint32> > ParseFile(QFile& inpfile, QString identity);

bool RealClientBD::CheckHosts(IP &ans, std::vector<QString> & working){
    //вот сюда надо положить функцию клиента БД, возрващающую IP упавшего хоста, если кто-то упал
    std::cout << "|BD:|\tEntered check hosts for BD" << std::endl;
    QString bufLog("CheckHosts entered\n");
    emit logStr(bufLog);

    vector<QString> failedIPs;
    try{
        if(!connected){
            _connect();
            connected = true;
        }
        failedIPs = pingAll();
    }
    catch(...){
        std::cout << std::endl<< "|BD:|\tUnable to reach Server"<< std::endl;
        bufLog = "Unable to reach Server\n";
        emit logStr(bufLog);
        return true;
    }

    /*for(QString each : failedIPs){
        working.push_back(each);
    }*/
    //std::cout << "|BD:|\tCheckhosts completed for BD" << std::endl;
    if(failedIPs.empty()){
        bufLog = "CheckHosts completed: Everything is OK\n";
        emit logStr(bufLog);
        return true;
    }
    else{
        ans = failedIPs.at(0);
        bufLog = "Disabled IP:"+ans.GetIP()+":"+ans.GetPort()+"\n CheckHosts Completed\n\n";
        emit logStr(bufLog);
        return false;
    }
}

void RealClientBD::SendConfig(IP &ans, std::vector<QString>& working){
    std::cout << "|BD:|\tentered SendConfig for BD" << std::endl;
    QString bufLog;
    bufLog = "Entered SendConfig\n";
    emit logStr(bufLog);
    addHost(ans.GetIP(),ans.GetPort());
    std::cout << "|BD:|\tcompleted SendConfig for BD" << std::endl;
    bufLog = "Сompleted SendConfig\n";
    emit logStr(bufLog);
}

void RealClientBD::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientDisp::CheckHosts(IP & ans, std::vector<QString>& working){
    QString bufLog;
    std::cout << "|DISP:|\tentered Checkhosts for Disp" << std::endl;
    bufLog = "CheckHosts entered\n";
    emit logStr(bufLog);

    vector<QString> badIPs;
    try{
        badIPs= Execution::Client::ping();
    }
    catch(...){
        bufLog = "Server Unavailable\n";
        emit logStr(bufLog);
        return true;
    }

    std::cout << "|DISP:|\tcompleted Checkhosts for Disp" << std::endl << std::endl;
    if(badIPs.empty()){
        bufLog = "CheckHosts completed - Everything is Ok\n\n";
        emit logStr(bufLog);
        return true;
    }
    else{
        bufLog = "Getting information about disabled IP\n";
        emit logStr(bufLog);
        ans.SetFullAddress(badIPs.at(0).left(badIPs.at(0).indexOf(":")),badIPs.at(0).right(badIPs.at(0).length()-badIPs.at(0).indexOf(":")-1).toInt());
        bufLog = "Disabled IP:"+ans.GetIP()+":"+ans.GetPort()+"\n CheckHosts Completed\n\n";
        emit logStr(bufLog);
        return false;
    }
}

void RealClientDisp::SendConfig(IP &good, std::vector<QString>& working){
    QString bufLog;
    std::cout << "|DISP:|\tentered SendConfig for Disp" << std::endl;
    bufLog = "Entered SendConfig\n";
    emit logStr(bufLog);
    vector<QString> vec;
    vec.push_back(good.GetIP()+":"+QString::number(good.GetPort()));
    //setNewChild(vec);
    std::cout << "|DISP:|\tcompleted SendConfig for Disp" << std::endl;
    bufLog = "Сompleted SendConfig\n";
    emit logStr(bufLog);
}

void RealClientDisp::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientRBT::CheckHosts(IP &ans, std::vector<QString>& working){
    QString bufLog;
    std::cout << "|RBT:|\tCheckHosts entered" << std::endl << std::endl;
    bufLog = "CheckHosts entered\n";
    emit logStr(bufLog);
    //вот сюда надо положить функцию клиента RBTree, возрващающую IP упавшего хоста, если кто-то упал
    QStringList workingHosts;
    bool carryON = true;
    try{
        workingHosts = getRingHosts();
        for(QString each : workingHosts){
            working.push_back(each);
        }
    }
    catch(...){
        std::cout << std::endl<< "|RBT:|\tUnable to reach Server"<< std::endl;
        bufLog = "Unable to reach Server\n\n";
        emit logStr(bufLog);
        carryON = false;
        working.clear();
        return true;
    }
    if(carryON){
        QFile confi("conf.txt");
        confi.open(QIODevice::ReadOnly);
        vector<pair<QString,qint32> > vecConfHosts = ParseFile(confi, "RBTHosts");
        confi.close();
        std::cout << "|RBT:|\tStarts looking for disabled IP" << std::endl;
        bufLog = "Starts looking for disabled IP\n";
        emit logStr(bufLog);
        if((qint32)vecConfHosts.size() == workingHosts.size()){
            bufLog = "completed Checkhosts - Everything is Ok\n";
            emit logStr(bufLog);
            return true;
        }
        else{
            for(pair<QString,qint32> bufConf : vecConfHosts){
                if(!workingHosts.contains(bufConf.first+":"+QString::number(bufConf.second))){
                    ans.SetFullAddress(bufConf.first,bufConf.second);
                    std::cout << std::endl << "|RBT:|\tDisabled IP found: " << ans.GetIP().toStdString() << ":" << ans.GetPort() << std::endl;
                    bufLog = "Disabled IP found:"+ ans.GetIP() + ":" + ans.GetPort() + "\n";
                    emit logStr(bufLog);
                }
            }
            bufLog = "completed Checkhosts - Everything is Ok\n";
            emit logStr(bufLog);
            //ans.SetFullAddress("192.168.1.1",1515);
            return false;
        }
    }
    return true;
}

void RealClientRBT::SendConfig(IP & good, std::vector<QString>& working){
    QString bufLog;
    // отправить изменённый конфигурационный файл SendConfig()
    std::cout << "|RBT:|\tentered SendConfig for RBT" << std::endl;
    bufLog = "Entered SendConfig\n";
    emit logStr(bufLog);
    QString por;
    por.setNum(good.GetPort());

    //std::cout << (good.GetIP()+":"+QString(good.GetPort())).toStdString();
    QStringList workingHosts;
    for(QString each : working){
        workingHosts.push_back(each);
    }
    joinToRing(good.GetIP()+":"+por, workingHosts);
    std::cout << "|RBT:|\tcompleted SendConfig for RBT" << std::endl;
    bufLog = "Сompleted SendConfig\n";
    emit logStr(bufLog);
}

void RealClientRBT::UpdateConfig(){}
