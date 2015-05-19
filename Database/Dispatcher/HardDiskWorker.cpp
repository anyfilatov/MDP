#ifndef HARDDISKWORKER_CPP
#define HARDDISKWORKER_CPP

#include <QDebug>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QRegExp>
#include <QStringList>
#include <vector>
#include "Dispatcher/HardDiskWorker.h"
#include "Dispatcher/MetaData.h"
#include "HashTable/TableKey.h"
#include "Data.h"

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
    maxFileSize = 30000;
    uploadMetaData();
}

HardDiskWorker::~HardDiskWorker(){

}

void HardDiskWorker::uploadMetaData(){
    meta.clear();
    QDir dir(DBSource);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (short i = 0; i < dir.count(); i++){
        QString name = dir[i];
        QStringList tokens = name.split(QRegExp("_"));
        short user_id = tokens[0].toInt();
        short data_id = tokens[1].toInt();
        short process_id = tokens[2].toInt();
        TableKey key(user_id, data_id, process_id);
        qDebug() << DBSource + name + "/meta";
        QFile metaFile(DBSource + name + "/meta");
        if (metaFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&metaFile);
            int size = in.readLine().toInt();
            tokens = in.readLine().split(QRegExp("\t"), QString::SkipEmptyParts);
            vector<QString> headers(tokens.size());
            for (int i = 0; i < tokens.size(); i++){
                headers[i] = tokens[i];
            }
            metaFile.close();
            MetaData* metaData = new MetaData(headers, size);
            qDebug() << metaData->serialize();
            meta.put(&key, metaData);
        }
    }
}

void HardDiskWorker::updateMetaFile(short int userId, short int dataId, short int processId, MetaData* metaData){
    QString name = QString::number(userId) + "_" + QString::number(dataId) + "_" + QString::number(processId);
    QFile metaFile(DBSource + name + "/meta");
    metaFile.open(QIODevice::WriteOnly);
    QTextStream out(&metaFile);
    out << metaData->getSize() << endl;
    for (int i = 0;  i < metaData->getHeaders().size(); i++){
        out << metaData->getHeaders()[i] << "\t";
    }
    metaFile.close();
}

void HardDiskWorker::putCells(short int userId, short int dataId, short int processId, int oldSize, const vector<vector<QString> > &cells){
    QString name = QString::number(userId) + "_" + QString::number(dataId) + "_" + QString::number(processId);
    int currentNum = 0;
    int currentFileId = oldSize / maxFileSize + 1;
    int rest = maxFileSize - oldSize % maxFileSize;
    while (currentNum < cells.size()){
        QFile file(DBSource + name + "/" + QString::number(currentFileId));
        if (!file.exists()){
            if (!file.open(QIODevice::WriteOnly)) {
                return;
            }
        }else{
            if (!file.open(QIODevice::Append)) {
                return;
            }
        }
        while ((rest > 0) && currentNum < cells.size()){
            QString line;
            for (int i = 0; i < cells[currentNum].size(); i++){
                line += cells[currentNum][i] + "\t";
            }
            line += "\n";
            file.write(line.toUtf8());
            rest--;
            currentNum++;
        }
        file.close();
        rest = maxFileSize;
        currentFileId++;
    }

}

bool HardDiskWorker::contains(TableKey & key){
    return meta.contains(&key);
}

int HardDiskWorker::getSize(TableKey & key){
    MetaData* metaData = meta.get(&key);
    if (metaData == NULL) return 0;
    return metaData->getSize();
}

const vector<QString>& HardDiskWorker::getHeaders(TableKey & key){
    MetaData* metaData = meta.get(&key);
    if (metaData == NULL){

    }
    metaData->getHeaders();
    return metaData->getHeaders();
}

int HardDiskWorker::getMaxFileSize(){
    return maxFileSize;
}

vector<TableKey>& HardDiskWorker::keys(){
    vector<TableKey> Keys = meta.keys();
    return Keys;
}

void HardDiskWorker::put(short int userId, short int dataId, short int processId, MDPData* data){
    if (data == NULL) return;
    TableKey key(userId, dataId, processId);
    MetaData* metaData = meta.get(&key);
    int oldSize = 0;
    if (metaData == NULL){
        metaData = new MetaData(data->getHeaders(), data->size());
        meta.put(&key, metaData);
        QDir dir(DBSource);
        QString name = QString::number(userId) + "_" + QString::number(dataId) + "_" + QString::number(processId);
        dir.mkdir(name);
    }else{
        vector <QString> headers = data->getHeaders();
        if (headers.size() != metaData->getHeaders().size())
            throw "ERROR! WRONG SIZE OF HEADERS";
        for (int i = 0; i < headers.size(); i++){
            if (headers[i] != metaData->getHeaders()[i])
                throw "ERROR! WRONG NAMES OF HEADERS";
        }
        oldSize = metaData->getSize();
        metaData->setSize(oldSize + data->size());
    }
    updateMetaFile(userId, dataId, processId, metaData);
    putCells(userId, dataId, processId, oldSize, data->getCells());
}

vector<vector<QString> > HardDiskWorker::get(short int userId, short int dataId, short int processId, int part){
    TableKey key(userId, dataId, processId);
    MetaData* metaData = meta.get(&key);
    vector<vector<QString> > cells;
    if (metaData == NULL){
        throw "ERROR! WRONG TABLE ID IN GET METHOD";
    };
    int lastPart = metaData->getSize() / maxFileSize;
    int size;
    if (part < lastPart && part > 0){
        size = maxFileSize;
    }else if (part == lastPart){
        size = metaData->getSize() % maxFileSize;
    }else{
        return cells;
    }
    cells.resize(size);
    QString name = QString::number(userId) + "_" + QString::number(dataId) + "_" + QString::number(processId);
    QFile inputFile(DBSource + name + "/" + QString::number(part));
    if (!inputFile.open(QIODevice::ReadOnly)){
        throw "ERROR! FILE DOESN'T EXIST";
    }
    bool flag = true;
    QTextStream in(&inputFile);
    int lines = 0;
    do{
       QString line = in.readLine();
       flag = line != NULL;
       if (flag){
            vector<QString> cortege;
            QStringList tokens = line.split("\t", QString::SkipEmptyParts);
            foreach(QString str, tokens) {
                cortege.push_back(str);
            }
//            qDebug() << cortege[1];
            cells[lines].swap(cortege);
        }
       lines++;
    }while (flag && lines < size);
    qDebug() << "lines: " << lines << ", size: " << cells.size();
    if (lines < size){
        cells.resize(lines);
    }
    inputFile.close();
    return cells;
}

#endif // HARDDISKWORKER_CPP
