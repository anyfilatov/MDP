#ifndef StringWithHash_h
#define StringWithHash_h

#include "HashTable/AbstractTableKey.h"
#include <QString>

using namespace std;

namespace database{

class StringWithHash:public AbstractTableKey{
private:
    QString value;

public:
    StringWithHash();
    StringWithHash(QString val);
    ~StringWithHash();
    QString& getValue();
    void setValue(QString val);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    QString serialize();
    void parse(QString json);
    struct comparator {
        bool operator() (StringWithHash& key1, StringWithHash& key2){
            return (key1.getValue() == key2.getValue());
        }
    };
};

}

#endif
