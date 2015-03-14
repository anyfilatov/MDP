#ifndef STORAGE
#define STORAGE
#include <QList>

template < typename K, typename V >
class Storage{
public:
    virtual void put(K,V) = 0;
    virtual V get(K) = 0;\
    virtual V remove(K) = 0;
    virtual QList<V> traverse(int start, int end) = 0;
    virtual QList<V> getAll() = 0;
};

#endif // STORAGE

