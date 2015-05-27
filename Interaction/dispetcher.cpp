#include "dispetcher.h"
#include "realclients.h"
#include <QThread>
#include <QObject>
#include <QFile>
#include <iostream>
#include <vector>
#include <QStringList>
#include <QTextStream>
#include "threadpinger.h"
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrentRun>
#include <QTextEdit>
using namespace integration;

std::vector<std::pair<QString,qint32> > ParseFile(QFile& inpfile, QString identity);
std::pair<QString, qint32> CreateNewConfig(qint32 num, std::pair<QString, qint32> badIP);

Dispetcher::Dispetcher(){
    AbstractClient* clientBD = new RealClientBD("169.254.99.102",2323);
    AbstractClient* clientRBT = new RealClientRBT("settings.json");
    AbstractClient* clientDisp = new RealClientDisp("169.254.99.101",4001);

    addL(clientBD);
    addL(clientRBT);
    addL(clientDisp);
    Resource resBD("ResourceForBD");
    Resource resRBT("ResourceForRBT");
    Resource resDisp("ResourceForDisp");
    addR(resBD);
    addR(resRBT);
    addR(resDisp);
    setLinks('L',0,std::vector<int>(1,0));
    setLinks('L',1,std::vector<int>(1,1));
    setLinks('L',2,std::vector<int>(1,2));

    carryOnChecking = true;
}

void Dispetcher::CheckClients(){
    /*while(carryOnChecking){
        //for(int i = 0; i < 3; i++){
        int i = 1;
            IP badIP;
            std::vector<QString> buffer;
            if(getL(i).getNode()->CheckHosts(badIP, buffer) == false){
                //посылаем обновлённый конфиг
                std::pair<QString,qint32> pairBadIp;
                pairBadIp.first = badIP.GetIP();
                pairBadIp.second = badIP.GetPort();
                std::pair<QString,qint32> goodIP = CreateNewConfig(i,pairBadIp);
                IP goodIPIP;
                goodIPIP.SetFullAddress(goodIP.first,goodIP.second);
                getL(i).getNode()->SendConfig(goodIPIP, buffer);

                //теперь убираем ресурс из свобоных в графе
                int ippos = 0;
                for (auto nod : getR(i).getNode().getAddresses()){
                    if (nod.GetIP().compare(goodIPIP.GetIP()) == 0 && nod.GetPort() == goodIPIP.GetPort()){
                        getR(i).getNode().getAddresses().erase(getR(i).getNode().getAddresses().begin()+ippos);

                    }
                    ippos++;
                }
                ;
                //for(int j = 0; j < 3; i++){
                //
                //    getL(j).getNode()->SendConfig();
                //    getL(j).getNode()->UpdateConfig();
                //}
                std::cout << std::endl;
            }
        //}
        QThread::sleep(2);
    }*/
   /* ThreadPinger PingerBD(this,0);
    PingerBD.start();
    ThreadPinger PingerDisp(this,1);
    PingerDisp.start();*/
    //QThread tRBT;
    ThreadPinger* PingerBD = new ThreadPinger(this,0);
    ThreadPinger* PingerRBT = new ThreadPinger(this,1);
    ThreadPinger* PingerDisp = new ThreadPinger(this,2);

    QtConcurrent::run(PingerBD,&ThreadPinger::run);
    QtConcurrent::run(PingerRBT,&ThreadPinger::run);
    QtConcurrent::run(PingerDisp,&ThreadPinger::run);

}

std::pair<QString,qint32> CreateNewConfig(qint32 num, std::pair<QString,qint32> badIP){
    QFile conf("conf.txt");
    conf.open(QIODevice::ReadOnly);
    std::vector<std::vector<std::pair<QString,qint32> > > allIp;
    while(!conf.atEnd()){
        QStringList tokens;
        QString bufstr = conf.readLine();
        tokens = bufstr.split(" ");
        std::vector<std::pair<QString,qint32> > bufvec;
        std::pair<QString,qint32> name(tokens.at(0),0);
        bufvec.push_back(name);
        tokens.erase(tokens.begin());
        foreach(QString s, tokens){

            QString ip = s.left(s.indexOf(":"));
            qint32 port = s.right(s.length()-s.indexOf(":")-1).toInt();

            std::pair<QString,qint32> item(ip,port);
            bufvec.push_back(item);
        }
        allIp.push_back(bufvec);
    }
    conf.close();
    //^---------Тут считали весь текущий config
    typedef std::pair<QString,qint32> Ippair;
    QString own = "";
    if(num == 0) own = "ResourceForBD";
    if(num == 1) own = "ResourceForRBT";
    if(num == 2) own = "ResourceForDisp";
    int posRes_i,posRes_j;
    int i = 0, j = 0;
    foreach (std::vector<Ippair > bufvec, allIp) {
        if(bufvec.at(0).first.compare(own) == 0){
            posRes_i = i;
            foreach (Ippair buf, bufvec) {
                if(buf.second == badIP.second)
                    posRes_j = j;
                j++;
            }
        }
        i++;
    }
    std::pair<QString,qint32> answer = allIp.at(posRes_i).at(posRes_j);
    i = 0;
    j = 0;
    foreach (std::vector<Ippair > bufvec, allIp) {
        j = 0;
        foreach (Ippair bufpair, bufvec) {
            if(bufpair.first.compare(badIP.first) == 0 && bufpair.second == badIP.second)
            {
                allIp.at(i).at(j).first = allIp.at(posRes_i).at(posRes_j).first;
                allIp.at(i).at(j).second = allIp.at(posRes_i).at(posRes_j).second;
                allIp.at(posRes_i).erase(allIp.at(posRes_i).begin() + posRes_j);
            }
            j++;
        }
        i++;
    }
    //^-------------Тут заменили упавший ip на ресурс. из ресурсов ip удалён
    conf.open(QIODevice::WriteOnly);
    QTextStream outstr(&conf);
    foreach (std::vector<Ippair > bufvec, allIp) {
        outstr << bufvec.at(0).first;
        bufvec.erase(bufvec.begin());
        foreach (Ippair bufpair, bufvec) {
            outstr << " " << bufpair.first << ":" << bufpair.second;
        }
        outstr << "\n";

    }
    conf.close();
    return answer;
}
