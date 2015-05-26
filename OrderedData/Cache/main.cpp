#include <QCoreApplication>
#include "cache.h"
#include <QDebug>
#include <QPair>
#include "RBTree.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Cache<QString, QPair<QString, bool> > cache;

    qDebug() << "insert: " << cache.insert("testKey", qMakePair<QString, bool>("test",false));
    qDebug() << "search: " << cache.search("testKey");
    qDebug() << "remove: " << cache.remove("testKey");
    qDebug() << "search: " << cache.search("testKey");

    RBTree<int, string> tree;
    tree.insert(1, "1");
    tree.insert(1, "1");
    tree.insert(3, "1");
    tree.insert(4, "1");

    foreach (int x, tree.getKeys()) {
        qDebug() << x;
     }

    return a.exec();
}


