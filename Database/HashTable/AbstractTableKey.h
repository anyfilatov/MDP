#ifndef TableKey_h
#define TableKey_h

#include "Clonable.h"
#include "HashTable/Hashable.h"
#include "Serializible.h"

class AbstractTableKey: public Clonable, public Hashable, public Serializible{
public:
    virtual bool equals(AbstractTableKey* other)= 0;
};

#endif
