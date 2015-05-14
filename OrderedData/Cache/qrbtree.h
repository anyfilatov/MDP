#ifndef QRBTREE_H
#define QRBTREE_H

#include "qrbnode.h"
#include <QMap>
#include <vector>

template<typename K, typename V>
class QRBTree
{
public:
    QRBTree();
    QRBTree(K key, V value);
    ~QRBTree() {}

    void insert(K key, V value, bool override = false);
    void insert(K key, QList<V> values);
    void replace(K key, QList<V> values);

    QRBNode<V>& getNode(K key);
    vector<V> search(K key);

    int remove(K key);
    int remove(K key, V value);
    void removeAll();

    QList<QRBNode<V>> nodes();
    QList<QRBNode<V>> replicaNodes();

    bool isEmpty() { return map.isEmpty(); }
    unsigned int size() { return map.size(); }

private:
    QMap<K,QRBNode<V>> map;
};

template<typename K, typename V>
QRBTree<K,V>::QRBTree()
{

}

template<typename K, typename V>
QRBTree<K,V>::QRBTree(K key, V value)
{
    map.insert(key, value);
}

template<typename K, typename V>
void QRBTree<K,V>::insert(K key, V value, bool override)
{
    if (map.contains(key)) {
        map[key].addValue(value, override);
    } else {
        map.insert(key, QRBNode<V>(value));
    }
}

template<typename K, typename V>
void QRBTree<K,V>::insert(K key, QList<V> values)
{
    if (map.contains(key)) {
        map[key].addValues(values.toVector().toStdVector());
    } else {
        map.insert(key, QRBNode<V>(values.toVector().toStdVector()));
    }
}

template<typename K, typename V>
void QRBTree<K,V>::replace(K key, QList<V> values)
{
    map.remove(key);
    map.insert(key, QRBNode<V>(values.toVector().toStdVector()));
}

template<typename K, typename V>
QRBNode<V>& QRBTree<K,V>::getNode(K key)
{
    return map[key];
}

template<typename K, typename V>
vector<V> QRBTree<K,V>::search(K key)
{
    return map[key].values();
}

template<typename K, typename V>
int QRBTree<K,V>::remove(K key)
{
    return map.remove(key);
}

template<typename K, typename V>
int QRBTree<K,V>::remove(K key, V value)
{
    map[key].removeValue(value);
    if (map[key].values().size() == 0)
        map.remove(key);
    return 0;
}

template<typename K, typename V>
void QRBTree<K,V>::removeAll()
{
    map.clear();
}

template<typename K, typename V>
QList<QRBNode<V>> QRBTree<K,V>::nodes()
{
    QList<QRBNode<V>> nodes;
    foreach (K key, map.keys()) {
        nodes.append(map.value(key));
    }
    return nodes;
}

template<typename K, typename V>
QList<QRBNode<V>> QRBTree<K,V>::replicaNodes()
{
    QList<QRBNode<V>> nodes;
    foreach (K key, map.keys()) {
        QRBNode<V> node = map[key];
        if (node.is_replica()) {
            nodes.append(map[key]);
        }
    }
    return nodes;
}

#endif // QRBTREE_H















