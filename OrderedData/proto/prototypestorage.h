#ifndef PROTOTIPESTORAGE_H
#define PROTOTIPESTORAGE_H
#include "storage.h"
#include <QMultiMap>
#include <iostream>

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

template <typename K, typename V>
PrototypeStorage<K,V>::PrototypeStorage()
{

}

template <typename K, typename V>
PrototypeStorage<K,V>::~PrototypeStorage()
{

}

template <typename K, typename V>
void PrototypeStorage<K,V>::put(K key, V value)
{
    map.insert(key, value);
}

template <typename K, typename V>
QList<V> PrototypeStorage<K,V>::get(K key)
{
    return map.values(key);
}

template <typename K, typename V>
int PrototypeStorage<K,V>::remove(K key)
{
    return map.remove(key);
}

template <typename K, typename V>
QList<V> PrototypeStorage<K,V>::getAll()
{
    return map.values();
}

#endif // PROTOTIPESTORAGE_H
