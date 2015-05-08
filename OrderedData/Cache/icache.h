#ifndef ICACHE_H
#define ICACHE_H

template<typename K, typename V>
class iCache
{
public:
    virtual int insert(K key, V value) = 0;
    virtual int insert(K key, QList<V> value) = 0;
    virtual V search(K key) = 0;
    virtual int remove(K key) = 0;
    virtual bool isEmpty() = 0;
    virtual int replace(K key, QList<V> values) =  0;
};

#endif // ICACHE_H
