#ifndef TABLEKEY
#define TABLEKEY

#include "AbstractTableKey.h"
#include <QString>

using namespace std;

class TableKey: public AbstractTableKey{
private:
    short int userId;
    short int dataId;
    short int processId;
public:
    TableKey();
    TableKey(short int userId, short int dataId);
    TableKey(short int userId, short int dataId, short int processId);
    ~TableKey();
    short int getUserId();
    void setUserId(short int userId);
    short int getDataId();
    void setDataId(short int dataId);
    short int getProcessId();
    void setProcessId(short int processId);
    void setKey(short int userId, short int dataId, short int processId);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    QString serialize();
    void parse(QString json);
};


#endif // TABLEKEY

