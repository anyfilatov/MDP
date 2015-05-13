#include "Data.h"
#include <string>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "GuiFileParser.h"

void GuiFileParser::setHeaders(vector<QString>* nheaders, vector<int>* nheadersNums)  {
    headers = nheaders;
    headersNums = nheadersNums;
}
void GuiFileParser::loadFile(QString fileName, DBClient& db){
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        bool flag = true;
        do{
            MDPData* datablock = packDataBlock(in, db);
            flag = datablock!=NULL && datablock->size()!=0;
            if (flag){
                db.put(userId,dataId,processId,datablock);
            }
            delete datablock;
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
    QString* line;
    do{
       QString lineconst =  inputStream.readLine();
       flag = lineconst!=NULL;
       if (flag){
            line =new QString(lineconst);
 //           qDebug()<<lineconst;
            QTextStream strin(line);
            QString value;
            bool valuesFlag = true;
            vector<QString> cortege;
            int i = 1;
            do{
                strin >> value;
                valuesFlag = value!=NULL;
                if (valuesFlag && std::find(headersNums->begin(), headersNums->end(),i)!=headersNums->end()){
                    cortege.push_back(value);
                }
                i++;
            }while (valuesFlag);
            celss.push_back(cortege);
            delete line;
        }
    }while (flag);
    MDPData* data = new MDPData(*headers, celss, 0);
    return data;
}
void GuiFileParser::setIds(int nuserId, int ndataId, int nprocessId=0){
    userId = nuserId;
    dataId = ndataId;
    processId = nprocessId;
}
