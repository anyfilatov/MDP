#ifndef TNODE_H
#define TNODE_H

#include <vector>

using namespace std;

enum Color { RED, BLACK };

template<typename K, typename V>
class TNode
{
public:
    TNode() {}
    TNode(K key, V value);
    TNode(K key, V value, Color color);
    TNode(K key, V value, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent);
    TNode(const TNode<K,V>& node);
    ~TNode() {}

    void addValue(V value);

    K key() const { return key_; }
    vector<V> values() const { return values_; }
    Color color() const { return color_; }
    TNode<K,V>* left() const { return left_; }
    TNode<K,V>* right() const { return right_; }
    TNode<K,V>* parent() const { return parent_; }

    void set_key(K key) { key_ = key; }
    void set_values(vector<V> values) { values_ = values; }
    void set_color(Color color) { color_ = color; }
    void set_left(TNode<K,V> *left) { left_ = left; }
    void set_right(TNode<K,V> *right) { right_ = right; }
    void set_parent(TNode<K,V> *parent) { parent_ = parent; }

private:
    K key_;
    vector<V> values_;
    Color color_;
    TNode<K,V> *left_;
    TNode<K,V> *right_;
    TNode<K,V> *parent_;

    void init(K key, V value, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent);
};

template<typename K, typename V>
TNode<K,V>::TNode(K key, V value)
{
    init(key, value, RED, NULL, NULL, NULL);
}

template<typename K, typename V>
TNode<K,V>::TNode(K key, V value, Color color)
{
    init(key, value, color, NULL, NULL, NULL);
}

template<typename K, typename V>
TNode<K,V>::TNode(K key, V value, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent)
{
    init(key, value, color, left, right, parent);
}

template<typename K, typename V>
TNode<K,V>::TNode(const TNode<K,V>& node)
{
    init(node.key(), node.values(), node.color(), node.left(), node.right(), node.parent());
}

template<typename K, typename V>
void TNode<K,V>::init(K key, V value, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent)
{
    key_ = key;
    values_.push_back(value);
    color_ = color;
    left_ = left;
    right_ = right;
    parent_ = parent;
}

template<typename K, typename V>
void TNode<K,V>::addValue(V value)
{
    values_.push_back(value);
}

#endif // TNODE_H
