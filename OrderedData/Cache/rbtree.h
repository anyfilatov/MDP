#ifndef RBTREE
#define RBTREE

#include "icache.h"
#include <QMap>
#include <QDebug>

template<typename K, typename V>
class RBTree : public iCache<K,V>
{
public:
    RBTree();
    virtual int insert(K key, V value);
    virtual V search(K key);
    virtual int remove(K key);
    virtual bool isEmpty();

private:
    QMap<K ,V> *rbtree;
};

template<typename K, typename V>
RBTree<K,V>::RBTree(){
    rbtree = new QMap<K ,V>;
}

template<typename K, typename V>
int RBTree<K,V>::insert(K key, V value){
    rbtree->insert(key, value);
    return 0;
}

template<typename K, typename V>
V RBTree<K,V>::search(K key){
    return rbtree->value(key);
}

template<typename K, typename V>
int RBTree<K,V>::remove(K key){
    return rbtree->remove(key);
}

template<typename K, typename V>
bool RBTree<K, V>::isEmpty() {
    rbtree->isEmpty();
}

#endif // RBTREE

