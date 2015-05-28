#ifndef GuiFileParser_h
#define GuiFileParser_h
#include <QString>
#include "Data.h"
#include "Dispatcher/Dispatcher.h"
#include "dbclient.h"

class GuiFileParser{
private:
    vector<QString>* headers;
    vector<int>* headersNums;
    int blockSize;
    int userId;
    int dataId;
    int processId;

    MDPData* packDataBlock(QTextStream& inputStream);
public:
       void setHeaders(vector<QString>* headers, vector<int>* headersNums);
       void loadFile(QString fileName, Dispatcher& db, int numberOfBlocks=INT_MAX);
       void loadFile(QString fileName, DBClient& db, int numberOfBlocks=INT_MAX);
       void setIds(int nuserId, int ndataId, int nprocessId);
       void setBlockSize(int blockSize);
};

#endif
