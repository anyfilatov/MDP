#pragma once
#include <QObject>
#include "dispetcher.h"
#include <iostream>

std::pair<QString,qint32> CreateNewConfig(qint32 num, std::pair<QString,qint32> badIP);
namespace integration{

    class ThreadPinger: public QObject{
        Q_OBJECT
    private:
        Dispetcher* disp;
        int id;
    public:
        ThreadPinger(Dispetcher* source, int whomToPing){
            disp = source;
            id = whomToPing;
        }
    public slots:
        void run(){
            while(true){
                IP badIP;
                std::vector<QString> buffer;
                if(disp->getL(id).getNode()->CheckHosts(badIP, buffer) == false){
                //посылаем обновлённый конфиг
                std::pair<QString,qint32> pairBadIp;
                pairBadIp.first = badIP.GetIP();
                pairBadIp.second = badIP.GetPort();
                std::pair<QString,qint32> goodIP = CreateNewConfig(id,pairBadIp);
                IP goodIPIP;
                goodIPIP.SetFullAddress(goodIP.first,goodIP.second);
                disp->getL(id).getNode()->SendConfig(goodIPIP, buffer);

                //теперь убираем ресурс из свобоных в графе
                int ippos = 0;
                for (auto nod : disp->getR(id).getNode().getAddresses()){
                    if (nod.GetIP().compare(goodIPIP.GetIP()) == 0 && nod.GetPort() == goodIPIP.GetPort()){
                        disp->getR(id).getNode().getAddresses().erase(disp->getR(id).getNode().getAddresses().begin()+ippos);
                        }
                        ippos++;
                    }
                    std::cout << std::endl;
                }

                QThread::sleep(2);
            }
            //std::cout << "weeee" << std::endl;
            emit finished();
        }

    signals:
        void finished();
    };



}

