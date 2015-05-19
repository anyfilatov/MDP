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
    short getUserId() const;
    void setUserId(short& userId);
    short getDataId() const;
    void setDataId(short& dataId);
    short getProcessId() const;
    void setProcessId(short& processId);
    void setKey(short& userId, short& dataId, short& processId);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    QString serialize();
    void parse(QString json);
    struct keys_comparator {
        bool operator() (const TableKey& key1, const TableKey& key2) const{
            return ((key1.getUserId() < key2.getUserId()) || (key1.getDataId() < key2.getDataId()) || (key1.getProcessId() < key2.getProcessId()));
        }
    };
};



#endif // TABLEKEY

