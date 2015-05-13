#ifndef GuiFileParser_h
#define GuiFileParser_h
#include <QString>
#include "dbclient.h"
#include "Data.h"

class GuiFileParser{
private:
    vector<QString>* headers;
    vector<int>* headersNums;
    int blockSize;
    int userId;
    int dataId;
    int processId;

    MDPData* packDataBlock(ifstream& ifstream, DBClient& db);
public:
       void setHeaders(vector<QString>* headers, vector<int>* headersNums);
       void loadFile(QString* fileName, DBClient& db);
       void setIds(int userId, int dataId, int processId=0);
       void setBlockSize(int blockSize);
};

#endif
