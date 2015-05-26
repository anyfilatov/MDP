#include <QCoreApplication>
#include "router.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Router router;
    router.startServer();
    return a.exec();
}

