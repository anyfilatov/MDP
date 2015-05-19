#ifndef HARDDISKWORKER_CPP
#define HARDDISKWORKER_CPP

#include <QDebug>
#include "Dispatcher/HardDiskWorker.h"
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QRegExp>
#include <QStringList>
#include <vector>
#include "HashTable/TableKey.h"

using namespace std;

HardDiskWorker* HardDiskWorker::p_instance = 0;
HardDiskWorkDestroyer HardDiskWorker::destroyer;

HardDiskWorkDestroyer::~HardDiskWorkDestroyer() {
    delete p_instance;
}

void HardDiskWorkDestroyer::initialize(HardDiskWorker* p) {
    p_instance = p;
}

HardDiskWorker& HardDiskWorker::getInstance() {
    if(!p_instance){
        p_instance = new HardDiskWorker();
        destroyer.initialize(p_instance);
    }
    return *p_instance;
}


HardDiskWorker::HardDiskWorker(){
    DBSource = "C:/MDPDatabase/";
    uploadTableKeys();
}

HardDiskWorker::~HardDiskWorker(){

}

void HardDiskWorker::uploadTableKeys(){
    tableKeys.clear();
    QDir dir(DBSource);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (short i = 0; i < dir.count(); i++){
        QString name = dir[i];
        qDebug() << name;
        QStringList tokens = name.split(QRegExp("_"));
        short user_id = tokens[0].toInt();
        short data_id = tokens[1].toInt();
        short process_id = tokens[2].toInt();
        qDebug() << user_id << ", " << data_id << ", " << process_id;
        TableKey key(user_id, data_id, process_id);
        tableKeys.insert(key);
    }
}

bool HardDiskWorker::contains(TableKey & key){
    return (tableKeys.find(key) != tableKeys.end());
}

vector<TableKey>& HardDiskWorker::keys(){
    vector<TableKey> Keys(tableKeys.begin(), tableKeys.end());
    return Keys;
}

#endif // HARDDISKWORKER_CPP
