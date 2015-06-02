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
    const QString& getValue() const;
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
    struct hashFunc {
        int operator() (const StringWithHash& key) const{
            return (key.getValue().length());
        }
    };
    struct cloneFunc {
        StringWithHash* operator() (const StringWithHash& key) const{
            return new StringWithHash(key.getValue());
        }
    };
};

}

#endif
