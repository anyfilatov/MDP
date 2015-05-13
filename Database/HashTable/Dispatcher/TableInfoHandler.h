#ifndef TABLEINFOHANDLER
#define TABLEINFOHANDLER

#include "Serializible.h"
#include <vector>

using namespace std;

class TableInfoHandler:public Serializible{
public:
    int stringsCount;
    vector<pair<int, int> > firstIndexes;
    TableInfoHandler(int count, vector<pair<int, int> > firstIndexes);
    ~TableInfoHandler();
    void putStrings(int num, int count);
};

#endif // TABLEINFOHANDLER

