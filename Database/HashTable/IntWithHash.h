#ifndef IntWithHash_h
#define IntWithHash_h

#include "AbstractTableKey.h"
#include <QString>

using namespace std;

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
};


#endif
