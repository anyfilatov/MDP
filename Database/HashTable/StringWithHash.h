#ifndef StringWithHash_h
#define StringWithHash_h

#include "AbstractTableKey.h"
#include <QString>

using namespace std;

class StringWithHash:public AbstractTableKey{
private:
    QString value;

public:
    StringWithHash();
    StringWithHash(QString val);
    ~StringWithHash();
    QString getValue();
    void setValue(QString val);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    QString serialize();
    void parse(QString json);
};

#endif
