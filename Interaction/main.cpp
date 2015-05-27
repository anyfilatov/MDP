#include <QCoreApplication>
#include <iostream>
#include "sstream"
#include <QFile>
#include <vector>
#include <QStringList>
#include "dispetcher.h"
#include "mywindow.h"
#include <typeinfo>
using namespace std;
using namespace integration;
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
    QApplication a(argc,argv);

    QFile conf("conf.txt");
    conf.open(QIODevice::ReadOnly);
    //vector<pair<QString,qint32> > ololo = ParseFile(conf,"Dispatcher");
    cout << "Hello World!";
    conf.close();

    integration::Dispetcher ddd;

    MyWindow1* mywin = new MyWindow1((integration::RealClientBD*)ddd.getL(0).getNode(),(integration::RealClientDisp*)ddd.getL(2).getNode(), (integration::RealClientRBT*)ddd.getL(1).getNode());
    //QObject::connect(mywin->sourceRecources,SIGNAL(logStr(QString)),mywin,SLOT(add(QString)));
    ddd.CheckClients();
    mywin->show();
    RealClientBD* i = new RealClientBD("192.164.200.200",2100);
    int k = 0;
    cout << endl << endl << endl << endl << ddd << endl << endl << endl << endl;
    //pair<QString,int> p("192.168.1.100",2020);
    //CreateNewConfig(0,p);
    //p.first = "192.168.1.102";
    //CreateNewConfig(0,p);
    return a.exec();
    /*integration::Resource I("RBTHosts");
    integration::Resource I1("RBTHosts");
    cout << (I == I1);*/

}
