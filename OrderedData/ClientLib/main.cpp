#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "ClientLib/client.h"
#include "server.h"



//int main(int argc, char** argv) {

//    QCoreApplication a(argc, argv);

//    if (!strcmp(argv[1], "1")) {
//        Server server;
//        server.run();
//    } else {

//        client client;
//        qDebug() << client.connectToHost("127.0.0.1", 12345);
//        QJsonObject object;
//        object.insert("key", "value");

//        client.write(object);
//        try {
//            QJsonObject repsonse = client.read();
//        } catch (...) {
//            qDebug() << "catched";
//        }
//    }

//    return a.exec();
//}



//QString getRandomString(int length)
//{
//    const QString alphabet(
//        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

//    const int randomStringLength = length;

//    QString randomString;
//    for (int i = 0; i < randomStringLength; ++i) {
//        int index = qrand() % alphabet.length();
//        QChar nextChar = alphabet.at(index);
//        randomString.append(nextChar);
//    }
//    return randomString;
//}

//QList<QPair<QString, QString> > generateStubData(int size, int keySize, int valueSize) {
//    QList<QPair<QString, QString> > randomKeyValuePairsList;
//    for (int i = 0; i < size; ++i) {
//        randomKeyValuePairsList << QPair<QString, QString>(getRandomString(keySize),
//                                                           getRandomString(valueSize));
//        if (i % 100000 == 0) {
//            qDebug() << i;
//        }
//    }
//    qDebug() << "Generated";
//    return randomKeyValuePairsList;
//}

//int main()
//{
//    //    QCoreApplication a(argc, argv);
//    RemoteClient client(100,"settings.json");


//    QList<QPair<QString, QString> > randomKeyValuePairsList = generateStubData(1000000, 10, 10);
//    for (QPair<QString, QString> keyValue: randomKeyValuePairsList) {
//        qDebug() << client.put(keyValue.first, keyValue.second);
//    }


//    //    return a.exec();
//}




#include <QCoreApplication>
#include <QDebug>
#include "client.h"
#include "Exception/exception.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    Client client(1, "settings.json");
    qDebug() << "===Test 1===";
    try{
        client.put("testInsert", "testValue");
        qDebug() << "SEARCH:" << client.get("testInsert");
        client.remove("testInsert");
        qDebug() << "SEARCH:" << client.get("testInsert");
        client.put("testInsert", "testValue");
        qDebug() << "SEARCH:" << client.get("testInsert");
        client.remove("testInsert");
        qDebug() << "SEARCH:" << client.get("testInsert");

//        qDebug() << "===Test 3===";
//        client.put("testInsert1", "testValue", "TestBucket");
//        qDebug() << "SEARCH BUCKET:" << client.getBucketKeys("TestBucket");
//        qDebug() << "SEARCH:" << client.get("testInsert1","TestBucket");
//        client.remove("testInsert1", "TestBucket");

//        qDebug() << "===Test 4===";
//        qDebug() << "SEARCH BUCKET:" << client.getBucketKeys("TestBucket");
//        qDebug() << "SEARCH:" << client.get("testInsert2","TestBucket");
//        qDebug() << "SEARCH:" << client.get("testInsert2","TestBucket");


    } catch(ClientExeption& e){
        qDebug() << e.printError();
    }


//    qDebug() << client.getRingHosts();
//    return a.exec();
}


