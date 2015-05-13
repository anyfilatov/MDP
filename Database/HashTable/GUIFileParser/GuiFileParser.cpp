#include "Data.h"
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>

void GuiFileParser::setHeaders(vector<QString>* nheaders, vector<int>* nheadersNums)  {
    headers = nheaders;
    headersNums = headersNums;
}
void GuiFileParser::loadFile(QString fileName, DBClient& db){
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        bool flag = true;
        do{
            MDPData* datablock = packDataBlock(in, db);
            flag = datablock!=null && datablock->size()!=0;
            if (flag){
                sendDataBlock(datablock);
            }
            delete(data);
        }while(flag);
        inputFile.close();
    }
}
void GuiFileParser::setBlockSize(int nblockSize){
    blockSize = nblockSize;
}

MDPData* GuiFileParser::packDataBlock(QTextStream& inputStream, DBClient& db){
    bool flag = true;
    vector<vector<QString>> celss;
    QString line;
    do{
        line = inputStream.getLine();
        QTextStream strin(line);
        QString value;
        bool values = true;
        vector<QString> cortege;
        int i = 1;
        do{
            strin >> value;
            values = value!=null;
            if (value!=null && nheadersNums->contains(i)){
                cortege.push(value);
            }
            i++;
        }while (values);
        cells.push(cortege);
    }while (flag);
    MDPData data(*headers, cells, 0);
    return data;
}
void GuiFileParser::setIds(int nuserId, int ndataId, int nprocessId=0){
    usetId = nuserId;
    dataId = ndataId;
    processId = nprocessId;
}
