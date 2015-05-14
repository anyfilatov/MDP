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
    TNode(K key, vector<V> values, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent);
    TNode(Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent);
    TNode(const TNode<K,V>& node);
    ~TNode() {}

    void addValue(V value);
    void removeValue(V value);
    void addValues(vector<V> values, bool override = false);

    K key() const { return key_; }
    vector<V> values() const { return values_; }
    Color color() const { return color_; }
    TNode<K,V>* left() const { return left_; }
    TNode<K,V>* right() const { return right_; }
    TNode<K,V>* parent() const { return parent_; }
    bool is_replica() const { return replica_; }

    void set_key(K key) { key_ = key; }
    void set_values(vector<V> values) { values_ = values; }
    void set_color(Color color) { color_ = color; }
    void set_left(TNode<K,V> *left) { left_ = left; }
    void set_right(TNode<K,V> *right) { right_ = right; }
    void set_parent(TNode<K,V> *parent) { parent_ = parent; }
    void set_replica(bool replica) { replica_ = replica; }

private:
    K key_;
    vector<V> values_;
    Color color_;
    TNode<K,V> *left_;
    TNode<K,V> *right_;
    TNode<K,V> *parent_;
    bool replica_;

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
    replica_ = false;
}

template<typename K, typename V>
TNode<K,V>::TNode(K key, vector<V> values, Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent)
{
    key_ = key;
    values_ = values;
    color_ = color;
    left_ = left;
    right_ = right;
    parent_ = parent;
    replica_ = false;
}

template<typename K, typename V>
TNode<K,V>::TNode(Color color, TNode<K,V> *left, TNode<K,V> *right, TNode<K,V> *parent)
{
    color_ = color;
    left_ = left;
    right_ = right;
    parent_ = parent;
    replica_ = false;
}

template<typename K, typename V>
void TNode<K,V>::addValue(V value)
{
    values_.push_back(value);
}

template<typename K, typename V>
void TNode<K,V>::removeValue(V value) {
    values_.erase(std::remove(values_.begin(), values_.end(), value), values_.end());
}

template<typename K, typename V>
void TNode<K,V>::addValues(vector<V> values, bool override)
{
    for (V value : values) {
        if (override) {
            if (std::find(values_.begin(), values_.end(), value) != values_.end()) {
                continue;
            }
        }
        values_.push_back(value);
    }
}

#endif // TNODE_H
