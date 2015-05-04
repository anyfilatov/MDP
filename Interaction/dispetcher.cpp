#include "dispetcher.h"
#include <QThread>
#include <iostream>
using namespace integration;


Dispetcher::Dispetcher(){
    AbstractClient* clientBD, *clientRBT, *clientDisp; //TODO: земенить AbstractClient на нормальньный
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
        /*for(int i = 0; i < 3; i++){
            IP badIP();
            if(getL(i).getNode()->CheckHosts(badIP)){
                //TODO определить по badIp и i, кто упал и что нужно

                for(int i = 0; i < 3; i++){
                    getL(i).getNode()->SendConfig();
                    getL(i).getNode()->UpdateConfig();
                }
            }
        }*/
        std::cout << "a\n";
        QThread::sleep(1);
    }
}
