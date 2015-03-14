#ifndef PROTOTIPESTORAGE_H
#define PROTOTIPESTORAGE_H

template <typename K, typename V>
class PrototypeStorage: public Storage<K,V>
{
public:
    PrototypeStorage();
    ~PrototypeStorage();
};

#endif // PROTOTIPESTORAGE_H
