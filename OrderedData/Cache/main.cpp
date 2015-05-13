#include <QCoreApplication>
#include "cache.h"
#include <QDebug>
#include <QPair>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Cache<QString, QPair<QString, bool> > cache;

    qDebug() << "insert: " << cache.insert("testKey", qMakePair<QString, bool>("test",false));
    qDebug() << "search: " << cache.search("testKey");
    qDebug() << "remove: " << cache.remove("testKey");
    qDebug() << "search: " << cache.search("testKey");

    return a.exec();
}


