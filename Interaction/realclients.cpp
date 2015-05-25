#include "realclients.h"
#include "QFile"
#include <iostream>
using namespace integration;
using std::vector;
using std::pair;

vector<pair<QString,qint32> > ParseFile(QFile& inpfile, QString identity);

bool RealClientBD::CheckHosts(IP &ans){
    //вот сюда надо положить функцию клиента БД, возрващающую IP упавшего хоста, если кто-то упал
    std::cout << "entered check hosts" << std::endl;
    vector<QString> failedIPs = pingAll();
    std::cout << "pingAll completed" << std::endl;
    if(failedIPs.empty()){
        return true;
    }
    else{
        ans = failedIPs.at(0);
        return false;
    }
}

void RealClientBD::SendConfig(IP &ans){
    addHost(ans.GetIP(),ans.GetPort());
    // отправить изменённый конфигурационный файл SendConfig()
}

void RealClientBD::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientDisp::CheckHosts(IP & ans){
    std::cout << "entered Checkhosts" << std::endl;
    //вот сюда надо положить функцию клиента Диспетчера, возрващающую IP упавшего хоста, если кто-то упал
    vector<QString> badIPs = Execution::Client::ping();
    std::cout << "ping passed" << std::endl;
    if(badIPs.empty())
        return true;
    else{
        ans.SetFullAddress(badIPs.at(0).left(badIPs.at(0).indexOf(":")),badIPs.at(0).right(badIPs.at(0).length()-badIPs.at(0).indexOf(":")-1).toInt());
        return false;
    }
}

void RealClientDisp::SendConfig(IP &good){
    vector<QString> vec;
    vec.push_back(good.GetIP()+":"+QString::number(good.GetPort()));
    //setNewChild(vec);
    // отправить изменённый конфигурационный файл SendConfig()
}

void RealClientDisp::UpdateConfig(){}

//---------------------------------------------------------------------------------------------------

bool RealClientRBT::CheckHosts(IP &ans){
    std::cout << "CheckHosts entered" << std::endl;
    //вот сюда надо положить функцию клиента RBTree, возрващающую IP упавшего хоста, если кто-то упал
    QStringList workingHosts = getRingHosts();
    std::cout << "getRing completed" << std::endl;
    std::cout << workingHosts.at(0).toStdString() << std::endl;
    QFile confi("conf.txt");
    confi.open(QIODevice::ReadOnly);
    vector<pair<QString,qint32> > vecConfHosts = ParseFile(confi, "RBTHosts");
    confi.close();
    if((qint32)vecConfHosts.size() == workingHosts.size())
        return true;
    else{
        for(pair<QString,qint32> bufConf : vecConfHosts){
            if(!workingHosts.contains(bufConf.first+":"+QString::number(bufConf.second)))
                ans.SetFullAddress(bufConf.first,bufConf.second);
        }

        //ans.SetFullAddress("192.168.1.1",1515);
        return false;
    }
}

void RealClientRBT::SendConfig(IP & good){
    // отправить изменённый конфигурационный файл SendConfig()
    QStringList buf = getRingHosts();


    QString por;
    por.setNum(good.GetPort());

    //std::cout << (good.GetIP()+":"+QString(good.GetPort())).toStdString();
    joinToRing(good.GetIP()+":"+por,buf);
}

void RealClientRBT::UpdateConfig(){}

