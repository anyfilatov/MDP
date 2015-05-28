#include "Data.h"
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "GuiFileParser.h"
#include "Dispatcher/Dispatcher.h"
#include "dbclient.h"

void GuiFileParser::setHeaders(vector<QString>* nheaders, vector<int>* nheadersNums)  {
    headers = nheaders;
    headersNums = nheadersNums;
}

void GuiFileParser::loadFile(QString fileName, Dispatcher& db, int numberOfBlocks){
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        bool flag = true;
        int blockCounter = 0;
        do{
            MDPData* datablock = packDataBlock(in);
            flag = datablock!=NULL && datablock->getCells().size()!=0 && blockCounter<numberOfBlocks;
            qDebug() << flag;
            if (flag){
                db.put(userId,dataId,processId,datablock);
                qDebug() << "put";
                blockCounter++;
            }
            delete datablock;
        }while(flag);
        inputFile.close();
    }
}
void GuiFileParser::loadFile(QString fileName, DBClient& db, int numberOfBlocks){
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        bool flag = true;
        int blockCounter = 0;
        do{
            MDPData* datablock = packDataBlock(in);
            flag = datablock!=NULL && datablock->getCells().size()!=0 && blockCounter<numberOfBlocks;
            qDebug() << flag;
            if (flag){
                db.put(userId,dataId,processId,datablock);
                qDebug() << "put";
                blockCounter++;
            }
            delete datablock;
        }while(flag);
        inputFile.close();
    }
}

void GuiFileParser::setBlockSize(int nblockSize){
    blockSize = nblockSize;
}

MDPData* GuiFileParser::packDataBlock(QTextStream& inputStream){
    bool flag = true;
    vector<vector<QString>> celss;
    QString* line;
    int lines = 0;
    do{
        QString lineconst =  inputStream.readLine();
        flag = lineconst!=NULL && lines<blockSize;
        if (flag){
            line =new QString(lineconst);
            QTextStream strin(line);
            bool valuesFlag = true;
            vector<QString> cortege;
            int i = 1;
            QStringList tokens= line->split(" ",QString::SkipEmptyParts);
            foreach( QString str, tokens) {
                if (std::find(headersNums->begin(), headersNums->end(), i) != headersNums->end()){
                    cortege.push_back(str);
                }
                i++;
            }
            celss.push_back(cortege);
            delete line;
        }
        lines++;
    }while (flag);
    MDPData* data = new MDPData(*headers, celss, 0);
    return data;
}
void GuiFileParser::setIds(int nuserId, int ndataId, int nprocessId=0){
    userId = nuserId;
    dataId = ndataId;
    processId = nprocessId;
}
