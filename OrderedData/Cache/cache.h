#ifndef CACHE
#define CACHE

#include "icache.h"
#include "rbtree.h"

template<typename K, typename V>
class Cache : public iCache<K,V>
{
public:
    Cache();
    virtual int insert(K key, V value);
    virtual V search(K key);
    virtual int remove(K key);
    virtual bool isEmpty();

private:
    iCache <K,V> *rbtree;
};

template<typename K, typename V>
Cache<K,V>::Cache(){
    rbtree = new RBTree<K, V>;
}

template<typename K, typename V>
int Cache<K,V>::insert(K key, V value){
    rbtree->insert(key, value);
    return 0;
}

template<typename K, typename V>
V Cache<K,V>::search(K key){
    return rbtree->search(key);
}

template<typename K, typename V>
int Cache<K,V>::remove(K key){
    return rbtree->remove(key);
}

template<typename K, typename V>
bool Cache<K, V>::isEmpty() {
    return rbtree->isEmpty();
}

#endif // CACHE

