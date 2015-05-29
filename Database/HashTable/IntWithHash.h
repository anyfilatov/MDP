#ifndef IntWithHash_h
#define IntWithHash_h

#include "HashTable/AbstractTableKey.h"
#include <QString>

using namespace std;

namespace database{

class IntWithHash: public AbstractTableKey{
private:
	int value;
public:
	IntWithHash();
	IntWithHash(int val);
    ~IntWithHash();
    int getValue();
    void setValue(int val);
	int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    QString serialize();
    void parse(QString json);
    struct comparator {
        bool operator() (IntWithHash& key1, IntWithHash& key2) const{
            return (key1.getValue() == key2.getValue());
        }
    };
};

}


#endif
