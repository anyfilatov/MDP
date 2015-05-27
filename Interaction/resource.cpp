#include "resource.h"
#include <QFile>
#include <QStringList>
using namespace integration;

Resource::Resource(const QString& owner){
    this->owner = owner;
    setResource();
}

IP Resource::popResource(qint32 port){
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

void Resource::setResource(){
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

bool Resource::operator ==(Resource & tmp) const {
    if(this->owner.compare(tmp.getOwner()) != 0)
        return false;
    bool ans = true;
    for(IP tmpIP1 : address){
        bool eachfound = false;
        for(IP tmpIP2 : tmp.getAddresses()){
            eachfound |= (tmpIP1.GetIP().compare(tmpIP2.GetIP()) == 0 && tmpIP1.GetPort() == tmpIP2.GetPort());
        }
        ans &=eachfound;
    }
    return ans;
}

bool Resource::operator !=(Resource & tmp) const {
    return !(*this == tmp);
}

QString Resource::getOwner(){
    return owner;
}

std::ostream& integration::operator << (std::ostream& ostr, Resource& res) {
    ostr << "Owner: " << res.getOwner().toStdString() << "\nIPs:";
    for(IP bufIP : res.getAddresses())
    ostr << bufIP.GetIP().toStdString() << ":" << bufIP.GetPort() << " ";
    return ostr;
}
