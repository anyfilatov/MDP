#include "prototypestorage.h"

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



