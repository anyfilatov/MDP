#include "dispetcher.h"
#include "realclients.h"
#include <QThread>
#include <QFile>
#include <iostream>
#include <vector>
#include <QStringList>
#include <QTextStream>
using namespace integration;

std::vector<std::pair<QString,qint32> > ParseFile(QFile& inpfile, QString identity);

Dispetcher::Dispetcher(){
    AbstractClient* clientBD = new RealClientBD();
    AbstractClient* clientRBT = new RealClientRBT();
    AbstractClient* clientDisp = new RealClientDisp();

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
    while(carryOnChecking){
        for(int i = 0; i < 3; i++){
            IP badIP;
            if(getL(i).getNode()->CheckHosts(badIP) == false){
                //TODO определить по badIp и i, кто упал и что нужно

                for(int i = 0; i < 3; i++){
                    getL(i).getNode()->SendConfig();
                    getL(i).getNode()->UpdateConfig();
                }
            }
        }
        QThread::sleep(1);
    }
}

void CreateNewConfig(qint32 num, std::pair<QString,qint32> badIP){
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

}