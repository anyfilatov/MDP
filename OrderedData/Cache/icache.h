#ifndef ICACHE_H
#define ICACHE_H

#include <QList>

template<typename K, typename V>
class iCache
{
public:
    virtual int insert(K key, V value, bool override = false, bool replica = false) = 0;
//    virtual int insert(K key, QList<V> values) = 0;
//    virtual int replace(K key, QList<V> values) =  0;
    virtual QList<V> search(K key) = 0;
    virtual int remove(K key) = 0;
    virtual int remove(K key, V value) = 0;
    virtual bool isEmpty() = 0;
    virtual int size() = 0;
    virtual int redNodesCount() = 0;
    virtual int blackNodesCount() = 0;
    virtual int primaryNodesCount() = 0;
    virtual int replicaNodesCount() = 0;
    virtual int treeHeight() = 0;
    virtual int valuesCount() = 0;

};

#endif // ICACHE_H
