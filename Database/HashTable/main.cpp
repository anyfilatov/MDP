#ifndef main_cpp
#define main_cpp

//#include <conio.h>
#include <fstream>
#include <QString>
#include <sys/stat.h>
#include <vector>
#include "HashTable/StringWithHash.h"
#include "HashTable/IntWithHash.h"
#include "HashTable/AbstractTableKey.h"
#include "HashTable/TableKey.h"
#include "HashTable/HashTable.h"
#include "HashTable/CellItem.h"
#include "Data.h"
#include "Dispatcher/Dispatcher.h"
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QCoreApplication>
#include <QObject>
#include "dbclient.h"
//#include <unistd.h>
#include <QAbstractSocket>
#include <QThread>

using namespace std;

int main(int argc, char *argv[])
{
    QTextStream cout(stdout);
    QCoreApplication app(argc, argv);
    /*HashTable<StringWithHash, IntWithHash> table(10, 0.3);
    IntWithHash* value1 = new IntWithHash(1);
    IntWithHash* value2 = new IntWithHash(2);
    IntWithHash* value3 = new IntWithHash(3);
    IntWithHash* value4 = new IntWithHash(4);
    StringWithHash key1("first"), key2("second"), key3("third"), key4("first");
    table.put(&key1, value1);
    table.put(&key1, value2);
    table.put(&key2, value2);
    table.put(&key3, value3);
    table.update(&key1, value4);
    key1.setValue("rerfe");
    vector<StringWithHash*> keys = table.keys();
    for (int i = 0; i < keys.size(); i ++){
        cout << "  n: " << i << "\n  key: " << keys[i]->serialize() << endl;
    }
    table.get(&key1);
    cout << table.get(&key4)->getValue() << endl;
    cout << table.get(&key3)->getValue() << endl;

    cout << "ENTRIES\n";
    vector<pair<StringWithHash*, IntWithHash> > entries = table.entries();
    for (int i = 0; i < entries.size(); i ++){
        cout << "  n: " << i << endl;
        cout << "  key: " << entries[i].first->serialize();
        cout << "  value: " << entries[i].second.getValue() << endl;
    }*/

    /*QJsonObject jsonObj, outerObj;
    QByteArray msg;
    jsonObj.insert("type", "PUT");
    jsonObj.insert("key", "sddsf");
    jsonObj.insert("value", "asaa");
    QJsonDocument jdoc(jsonObj);
    QString str(jdoc.toJson());
    outerObj.insert("key1", str);
    QJsonDocument jdoc1(outerObj);
    cout << jdoc1.toJson() << endl;
    jdoc = jdoc.fromJson(jdoc1.toJson());
    outerObj = jdoc.object();
    QJsonValue val = outerObj.take("key1");
    str = val.toString();
    cout << str;
    TableKey key(2, 3, 0);
    cout << key.serialize() << endl;
    key.parse(key.serialize());
    cout << key.serialize() << endl;*/

    /*QCoreApplication a(argc, argv);
    Dispatcher dispatcher;
    dispatcher.startServer(1234);
    return a.exec();*/

    /*cout << table.serialize() << endl;
    HashTable<TableKey, HashTable<StringWithHash,IntWithHash> > table1(10, 0.3);
    TableKey key(2,3,0);
    table1.put(&key, &table);
    HashTable<TableKey, HashTable<StringWithHash,IntWithHash> > table2(10, 0.3);
    table2.parse(table1.serialize());
    cout << "table1 " << table1.serialize() << endl;
    cout << "table2 " << table2.serialize() << endl;
    cout << "table " << table2.get(&key)->serialize() << endl;*/



    /*HashTable<StringWithHash, IntWithHash>::Iterator iter = table.iterator();
    cout << iter.getValue()->getValue() << endl;
    while(iter.hasNext()){
        IntWithHash* val = iter.next();
        cout << val->getValue() << endl;
    }*/
    int dim = 10;
    vector<QString> headers(3);
    vector<vector<QString> > cells(dim);
    headers[0] = "firstaeiurblsrun";
    headers[1] = "secersgsregsergond";
    headers[2] = "tegersgsergregserhird";

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < 3; j++){
            cells[i].push_back(QString::number(i) + QString::number(j));
        }
    }


    MDPData data1(headers, cells, 0);
    MDPData data2(headers, cells, 0);
    MDPData data(headers, cells, 0);
    //data.parse(data.serialize());
    //data.serialize();
    //MDPData data;

    Dispatcher dispatcher(2323);
    dispatcher.put(0, 3, 2, &data);
    dispatcher.put(0,2,1, &data1);
    dispatcher.put(2, 2, 1, &data2);
    QJsonArray keys = dispatcher.getUsers();
    qDebug() << keys;
    /*qDebug() << dispatcher.getSize(0,3,2);
    MDPData* d = dispatcher.get(0, 3, 2, 3, 10);
    if (d != NULL){
        qDebug() << d->serialize();
    }*/

    /*DBClient client("172.19.25.19", 2323);

    MDPData* d = client.get(0, 3, 2);
    if (d != NULL){
        qDebug() <<d->serialize();
    }else {
        qDebug() << "NULL";
    }
    client.remove(0, 3, 2);
    d = client.get(0, 3, 2);
    if (d != NULL){
        qDebug() <<d->serialize();
    }else {
        qDebug() << "NULL";
    }*/

    /*d = client.get(0, 3, 2, 3, 15);
    if (d != NULL){
        qDebug() <<d->serialize();
    }*/
    //qDebug() << client.getSize(0, 3, 2);

    /*Data* newData = dispatcher.get(0, 3, 2);
    newData->serialize();
    newData = dispatcher.get(0, 3, 2, 1, 2);
    if (newData != NULL)
        newData->serialize();
    for (int i = 0; i < 4; i++){
        Data* newData = dispatcher.getNextStrings(0, 3, 2, 2);
        if (newData != NULL)
            newData->serialize();
    }*/

    return app.exec();
}

#endif
