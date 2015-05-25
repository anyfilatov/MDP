#include "resource.h"
#include <QFile>
#include <QStringList>
using namespace integration;

Resource::Resource(const QString& owner){
    this->owner = owner;
    SetResource();
}

IP Resource::PopResource(qint32 port){
    if(!address.empty()){
        /*IP res = address[0];
        address.erase(address.begin());
        return res;*/
        int i = 0;
        foreach(IP buf,address){
            if(buf.GetPort() == port){
                IP res = address[i];
                address.erase(address.begin()+i);
                return res;
            }
            i++;
        }
        return IP();
    }
    else{
        return IP();
    }
}

void Resource::SetResource(){
    QFile conf("conf.txt");
    conf.open(QIODevice::ReadOnly);
    QStringList tokens;
    do{
        QString bufstr = conf.readLine();
        tokens = bufstr.split(" ");
    } while(tokens[0].compare(owner) != 0 && !conf.atEnd());
    tokens.erase(tokens.begin());
    foreach(QString s, tokens){

        QString ip = s.left(s.indexOf(":"));
        qint32 port = s.right(s.length()-s.indexOf(":")-1).toInt();

        IP item(ip,port);
        address.push_back(item);
    }
}

std::vector<IP>& Resource::getAddresses(){
    return address;
}
