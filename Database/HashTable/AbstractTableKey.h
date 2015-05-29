#ifndef AbstractTableKey_h
#define AbstractTableKey_h

#include "Clonable.h"
#include "HashTable/Hashable.h"
#include "Serializible.h"

namespace database{

class AbstractTableKey: public Clonable, public Hashable, public Serializible{
public:
    virtual bool equals(AbstractTableKey* other)= 0;
};

}

#endif
