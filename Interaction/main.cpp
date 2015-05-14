#include <QCoreApplication>
#include <iostream>
#include "sstream"
#include <QFile>
#include <vector>
#include <QStringList>
#include "dispetcher.h"
using namespace std;
void CreateNewConfig(qint32 num, std::pair<QString,qint32> badIP);

vector<pair<QString,qint32> > ParseFile(QFile& inpfile, QString identity){
    QStringList tokens;
    do{
        QString bufstr = inpfile.readLine();
        tokens = bufstr.split(" ");
    } while(tokens[0].compare(identity) != 0);
    tokens.erase(tokens.begin());
    vector<pair<QString,qint32> > result;
    foreach(QString s, tokens){

        QString ip = s.left(s.indexOf(":"));
        qint32 port = s.right(s.length()-s.indexOf(":")-1).toInt();

        pair<QString,qint32> item(ip,port);
        result.push_back(item);
    }
    return result;
}

int main(int argc, char *argv[])
{
    QFile conf("conf.txt");
    conf.open(QIODevice::ReadOnly);
    vector<pair<QString,qint32> > ololo = ParseFile(conf,"Dispatcher");
    cout << "Hello World!";
    conf.close();

    integration::Dispetcher ddd;
    ddd.CheckClients();
    //pair<QString,int> p("192.168.1.100",2020);
    //CreateNewConfig(0,p);
    //p.first = "192.168.1.102";
    //CreateNewConfig(0,p);

}
