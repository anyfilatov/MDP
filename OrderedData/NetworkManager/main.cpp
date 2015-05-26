#include <QCoreApplication>
#include <QDebug>

#include "NetworkSettings.h"
#include "networkmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QFile settingsFile("settings.json");
    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    NetworkSettings networkSettings(settingsFile);
    settingsFile.close();
    
    qDebug() << networkSettings.getGossipNodes();
    

    NetworkManager manager(networkSettings);
    
//    qDebug() << manager.getMyself().getAddress();
//    for(LocalNode node: manager.getMemberList()) {
//        qDebug() << node.getAddress();
//    }
    
    return a.exec();
}
