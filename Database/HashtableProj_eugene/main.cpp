#ifndef main_cpp
#define main_cpp

//#include <conio.h>
#include <fstream>
#include <QString>
#include <vector>
#include "StringWithHash.h"
#include "IntWithHash.h"
#include "AbstractTableKey.h"
#include "HashTable.h"
//#include "JsonMethods.h"
#include "Data.h"
#include "dispatcher.h"
#include "dbclient.h"
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QTextStream cout(stdout);
    QCoreApplication app(argc, argv);
    /*HashTable<StringWithHash, IntWithHash> table(10, 0.3);
    IntWithHash value1(1), value2(2), value3(3), value4(4);
    StringWithHash key1("first"), key2("second"), key3("third"), key4("first");

    table.put(&key1, &value1);
    table.put(&key1, &value2);
    table.put(&key2, &value2);
    table.put(&key3, &value3);
    table.get(&key1);
    table.get(&key2);
    table.update(&key2, &value4);
    IntWithHash* val = table.get(&key2);
    val->setValue(200);
    key1.setValue("rerfe");
    //table.remove(&key2);
    cout << "KEYS\n";
    vector<StringWithHash*> keys = table.keys();
    for (int i = 0; i < keys.size(); i ++){
        cout << "  n: " << i << "\n  keyHash: " << keys[i]->hash() << endl;
    }
    table.get(&key1);
    table.get(&key4);
    cout << "ENTRIES\n";
    vector<pair<StringWithHash*, IntWithHash> > entries = table.entries();
    for (int i = 0; i < entries.size(); i ++){
        cout << "  n: " << i << "\n  keyHash: " << entries[i].first->hash();
        cout << "  value: " << entries[i].second.getValue() << endl;
    }*/


    /*
    QJsonObject jsonObj, outerObj;
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
    cout << key.serialize() << endl;
    */

    /*cout << QString::fromStdString(table.serialize()) << endl;
    HashTable<StringWithHash, HashTable<StringWithHash,IntWithHash> > table1(10, 0.3);
    table1.put(&key1, &table);
    HashTable<StringWithHash, HashTable<StringWithHash,IntWithHash> > table2(10, 0.3);
    table2.parse(table1.serialize());
    cout << QString::fromStdString(table1.serialize()) << endl;
    cout << QString::fromStdString(table2.serialize()) << endl;*/

    /*HashTable<StringWithHash, IntWithHash>::Iterator iter = table.iterator();
    cout << iter.getValue()->getValue() << endl;
    while(iter.hasNext()){
        IntWithHash* val = iter.next();
        cout << val->getValue() << endl;
    }*/

    vector<QString> headers(3);
    vector<vector<QString> > cells(3);
    headers[0] = "first";
    headers[1] = "second";
    headers[2] = "third";

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            cells[i].push_back(QString::number(i) + QString::number(j));
        }
    }

    Data data(headers, cells, 12);

    Dispatcher dispatcher(2324);

    QJsonObject jso;
    jso.insert("command", "somecommand");
    jso.insert("data", data.serialize());

    DBClient dbclient("127.0.0.1", 2324);
    dbclient.sendToServer(jso);

    /*
    dispatcher.put(0, 3, 2, &data);
    Data* newData = dispatcher.get(0, 3, 2);
    newData->serialize();
    newData = dispatcher.get(0, 3, 2, 1, 2);
    if (newData != NULL)
        newData->serialize();
    for (int i = 0; i < 4; i++){
        Data* newData = dispatcher.getNextStrings(0, 3, 2, 2);
        if (newData != NULL)
            newData->serialize();
    }


*/
    //system("pause");

    return app.exec();
    //return 0;
}

#endif
