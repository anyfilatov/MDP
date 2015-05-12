#ifndef CACHE
#define CACHE

#include "icache.h"
#include "rbtree.h"
#include "Router/StatusCodes.h"
#include <QList>

template<typename K, typename V>
class Cache : public iCache<K,V>
{
public:
    Cache();
    virtual int insert(K key, V value);
    virtual QList<V> search(K key);
    virtual int remove(K key);
    virtual bool isEmpty();

private:
    RBTree<K,V> *rbtree;
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
QList<V> Cache<K,V>::search(K key){
    QList<V> values;
    for(V value : rbtree->searchValues(key)) {
        values.push_back(value);
    }
    return values;
}

template<typename K, typename V>
int Cache<K,V>::remove(K key){
//    rbtree->remove(key);
    return 0;
}

template<typename K, typename V>
bool Cache<K, V>::isEmpty() {
    return rbtree->isEmpty();
}

#endif // CACHE

