#ifndef ICACHE_H
#define ICACHE_H

template<typename K, typename V>
class iCache
{
public:
    virtual int insert(K key, V value);
    virtual V search(K key);
    virtual int remove(K key);
};

#endif // ICACHE_H
