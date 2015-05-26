#ifndef CACHE
#define CACHE

#include "icache.h"
#include "rbtree.h"
#include "ClientLib/StatusCodes.h"
#include <QList>
#include <QVector>

template<typename K, typename V>
class Cache : public iCache<K,V>
{
public:
    Cache();
    virtual int insert(K key, V value, bool rewrite = false, bool replica = false);
//    virtual int insert(K key, QList<V> values);
//    virtual int replace(K key, QList<V> values);
    virtual QList<V> search(K key);
    virtual int remove(K key);
    virtual int remove(K key, V value);
    virtual bool isEmpty();
    virtual unsigned int size();
    virtual RBTree<K, V> *getRBTree();
private:
    RBTree<K,V> *rbtree;
};

template<typename K, typename V>
Cache<K,V>::Cache(){
    rbtree = new RBTree<K, V>;
}

template<typename K, typename V>
int Cache<K,V>::insert(K key, V value, bool rewrite, bool replica){
    rbtree->insert(key, value, rewrite, replica);
    return 0;
}

//template<typename K, typename V>
//int Cache<K,V>::insert(K key, QList<V> values){
//    rbtree->insert(key, values);
//    return 0;
//}

//template<typename K, typename V>
//int Cache<K,V>::replace(K key, QList<V> values){
//    rbtree->replace(key, values);
//    return 0;
//}

template<typename K, typename V>
QList<V> Cache<K,V>::search(K key){
    return QVector<V>::fromStdVector(rbtree->search(key)).toList();
}

template<typename K, typename V>
int Cache<K,V>::remove(K key){
    rbtree->remove(key);
    return 0;
}

template<typename K, typename V>
int Cache<K,V>::remove(K key, V value){
    rbtree->remove(key, value);
    return 0;
}

template<typename K, typename V>
bool Cache<K, V>::isEmpty() {
    return rbtree->isEmpty();
}

template<typename K, typename V>
unsigned int Cache<K, V>::size()
{
    return rbtree->size();
}

template<typename K, typename V>
RBTree<K, V> *Cache<K, V>::getRBTree()
{
    return rbtree;
}

#endif // CACHE

