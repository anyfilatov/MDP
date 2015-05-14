#include <QCoreApplication>
#include <QDebug>
#include "client.h"
#include "Exception/exception.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    Client client("settings.json");
    qDebug() << "===Test 1===";
    try{
        qDebug() << "INSERT:" << client.put("testInsert", "testValue");
        qDebug() << "SEARCH:" << client.get("testInsert");
        qDebug() << "REMOVE:" << client.remove("testInsert");
        qDebug() << "SEARCH:" << client.get("testInsert");

        qDebug() << "===Test 2===";
        QList<QString> list;
        list << "test1" << "test2" << "test3";
        qDebug() << "INSERT:" << client.put("testInsert", list);
        qDebug() << "SEARCH:" << client.get("testInsert");
        list << "test 4" << "test 5";
        qDebug() << "REPLACE:" << client.replace("testInsert", list);
        qDebug() << "SEARCH:" << client.get("testInsert");

//        qDebug() << "===Test 3===";
//        qDebug() << "INSERT:" << client.put("testInsert", "testValue", "TestBucket");
//        qDebug() << "INSERT:" << client.put("testInsert", "testValue1", "TestBucket");
//        qDebug() << "SEARCH:" << client.getBucketKeys("TestBucket");
//        qDebug() << "REMOVE:" << client.removeBucket("testInsert");
//        qDebug() << "SEARCH:" << client.getBucketKeys("TestBucket");

    } catch(ClientExeption& e){
        qDebug() << e.printError();
    }


//    qDebug() << client.getRingHosts();
//    return a.exec();
}


