#include <QCoreApplication>
#include "cacheserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CacheServer server;
    return a.exec();
}

