#ifndef PROTOTIPESTORAGE_H
#define PROTOTIPESTORAGE_H

#include "storage.h"
#include <QMap>

template <typename K, typename V>
class PrototypeStorage: public Storage<K,V>
{
public:
    PrototypeStorage();
    ~PrototypeStorage();

    void put(K, V);
    V get(K);
    V remove(K);
    QList<V> traverse(int start, int end);
    QList<V> getAll();

private:
    QMap<K,V> map;
};

#endif // PROTOTIPESTORAGE_H
