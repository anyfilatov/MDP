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
    QList<QPair<K, QList<V> > > getAll();

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
QList<QPair<K, QList<V> > > PrototypeStorage<K,V>::getAll()
{
    QList<QPair<K, QList<V> > > list;
    foreach (K key, map.keys()) {
        list.append(QPair<K, QList<V> >(key, map[key]));
    }
    return list;
}


#endif // PROTOTIPESTORAGE_H
