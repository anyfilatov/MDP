#ifndef STORAGE
#define STORAGE
#include <QList>

template < typename K, typename V >
class Storage{
public:
    virtual void put(K,V) = 0;
    virtual QList<V> get(K) = 0;
    virtual int remove(K) = 0;
    virtual QList<V> getAll() = 0;
};

#endif // STORAGE

