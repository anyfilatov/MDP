#ifndef CACHE
#define CACHE

#include "icache.h"

template<typename K, typename V>
class Cache : public iCache<K,V>
{
public:
    Cache();
    virtual int insert(K key, V value);
    virtual V search(K key);
    virtual int remove(K key);

private:
    iCache<K,V> *rbtree;
};

#endif // CACHE

