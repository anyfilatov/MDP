#include <QCoreApplication>
#include "cacheserver.h"
#include "NetworkManager/NetworkSettings.h"
#include "Router/hashring.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile settingsFile("settings.json");
    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Settings file not found";
        return -1;
    }

    NetworkSettings networkSettings(settingsFile);
    settingsFile.close();

    CacheServer cacheServer(networkSettings);

    return a.exec();
}
