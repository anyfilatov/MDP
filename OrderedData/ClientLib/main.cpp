#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "remoteclient.h"
#include "Exception/exception.h"

QString getRandomString(int length)
{
    const QString alphabet(
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    const int randomStringLength = length;

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i) {
        int index = qrand() % alphabet.length();
        QChar nextChar = alphabet.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

QList<QPair<QString, QString> > generateStubData(int size, int keySize, int valueSize) {
    QList<QPair<QString, QString> > randomKeyValuePairsList;
    for (int i = 0; i < size; ++i) {
        randomKeyValuePairsList << QPair<QString, QString>(getRandomString(keySize),
                                                           getRandomString(valueSize));
        if (i % 100000 == 0) {
            qDebug() << i;
        }
    }
    qDebug() << "Generated";
    return randomKeyValuePairsList;
}

int main()
{
    //    QCoreApplication a(argc, argv);
    RemoteClient client(100,"settings.json");


    QList<QPair<QString, QString> > randomKeyValuePairsList = generateStubData(1000000, 10, 10);
    for (QPair<QString, QString> keyValue: randomKeyValuePairsList) {
        qDebug() << client.put(keyValue.first, keyValue.second);
    }


    //    return a.exec();
}




/*#include <QCoreApplication>
#include <QDebug>
#include "client.h"
#include "Exception/exception.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
/*    Client client("settings.json");
    qDebug() << "===Test 1===";
    try{
        qDebug() << "INSERT:" << client.put("testInsert", "testValue");
        qDebug() << "SEARCH:" << client.get("testInsert");
        qDebug() << "REMOVE:" << client.remove("testInsert");
        qDebug() << "SEARCH:" << client.get("testInsert");
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
        qDebug() << "REMOVE:" << client.remove("testInsert");
        qDebug() << "SEARCH:" << client.get("testInsert");

        qDebug() << "===Test 3===";
        qDebug() << "INSERT BUCKET:" << client.put("testInsert1", "testValue", "TestBucket");
        qDebug() << "SEARCH BUCKET:" << client.getBucketKeys("TestBucket");
        qDebug() << "SEARCH:" << client.get("testInsert1","TestBucket");
        qDebug() << "REMOVE:" << client.remove("testInsert1", "TestBucket");

        qDebug() << "===Test 4===";
        qDebug() << "INSERT BUCKET:" << client.put("testInsert2", list, "TestBucket");
        qDebug() << "SEARCH BUCKET:" << client.getBucketKeys("TestBucket");
        qDebug() << "SEARCH:" << client.get("testInsert2","TestBucket");
        list.clear();
        list << "test6" << "test7";
        qDebug() << "REPLACE:" << client.replace("testInsert2", list, "TestBucket");
        qDebug() << "SEARCH:" << client.get("testInsert2","TestBucket");


    } catch(ClientExeption& e){
        qDebug() << e.printError();
    }


//    qDebug() << client.getRingHosts();
//    return a.exec();
}*/


