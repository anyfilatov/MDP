#include <QCoreApplication>
#include "cache.h"
#include <QDebug>
#include <QPair>
#include "RBTree.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
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
    tree.insert(15, "1");
    tree.insert(196, "1");
    tree.insert(42, "1");
    tree.insert(13, "1");
    tree.insert(34, "1");
    tree.insert(49, "1");
    tree.insert(45, "1");
    tree.insert(213, "1");
    tree.insert(334, "1");
    tree.insert(499, "1");
    tree.insert(149, "1");
    tree.insert(145, "1");
    tree.insert(273, "1");
    tree.insert(-14, "1");
    tree.insert(9, "1");
    tree.insert(123, "1");
    tree.insert(312, "1");
    tree.insert(41, "1");
    tree.insert(32, "1");
    tree.insert(76, "1");
    tree.insert(-1, "1");
    tree.insert(-98, "1");
    foreach (int x, tree.getKeys()) {
        qDebug() << x;
    }

    qDebug() << "size = " << tree.size();
    qDebug() << "red = " << tree.redNodesCount();
    qDebug() << "black = " << tree.blackNodesCount();
    qDebug() << "primary = " << tree.primaryNodesCount();
    qDebug() << "replica = " << tree.replicaNodesCount();
    qDebug() << "height = " << tree.treeHeight();
    qDebug() << "values = " << tree.valuesCount();
    qDebug() << "root = " << tree.root()->key();

    return 0;
}


