#ifndef PROTOTIPESTORAGE_H
#define PROTOTIPESTORAGE_H

#include "storage.h"
#include <QMultiMap>

template <typename K, typename V>
class PrototypeStorage: public Storage<K,V>
{
public:
    PrototypeStorage();
    ~PrototypeStorage();

    void put(K, V);
    QList<V> get(K);
    int remove(K);
    QList<V> getAll();

private:
    QMultiMap<K,V> map;
};

#endif // PROTOTIPESTORAGE_H
