#ifndef PROTOTIPESTORAGE_H
#define PROTOTIPESTORAGE_H
#include "storage.h"
template <typename K, typename V>
class PrototypeStorage: public Storage<K,V>
{
public:
    PrototypeStorage();
    ~PrototypeStorage();
};

template<typename K, typename V>
PrototypeStorage<K,V>::PrototypeStorage()
{

}
template<typename K, typename V>
PrototypeStorage<K,V>::~PrototypeStorage()
{

}

#endif // PROTOTIPESTORAGE_H
