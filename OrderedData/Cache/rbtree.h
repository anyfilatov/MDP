#ifndef RBTREE_H
#define RBTREE_H

#include "tnode.h"
#include <iostream>
#include <vector>
#include <QList>

using namespace std;

template<typename K, typename V>
class RBTree
{
public:
    RBTree();
    RBTree(K key, V value);
    ~RBTree();

    void insert(TNode<K,V> *z);
    void insert(K key, V value);
    void insert(K key, vector<V> values);
    void replace(K key, vector<V> values);

    void remove(K key);
    TNode<K,V>* remove(TNode<K,V> *z); // don't delete memory
    void removeAll();

    TNode<K,V>* search(TNode<K,V> *x, K key);
    TNode<K,V>& getNode(K key);
    vector<V> search(K key);

    void inorderWalk(TNode<K,V> *x, std::ostream_iterator<K> &iter);
    QList<TNode<K,V>*> nodes();
    QList<TNode<K,V>*> replicaNodes();

    bool isEmpty() { return size_ == 0 ? true : false; }
    unsigned int size() { return size_; }

    TNode<K,V>* root() { return root_; }

private:
    TNode<K,V> *root_;
    TNode<K,V> *nil_;
    unsigned int size_;

    void leftRotate(TNode<K,V> *x);
    void rightRotate(TNode<K,V> *x);
    void insertFixup(TNode<K,V> *z);
    TNode<K,V>* successor(TNode<K,V> *x);
    TNode<K,V>* minimum(TNode<K,V> *x);
    void removeFixup(TNode<K,V> *x);
    void inorderWalkNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list);
    void inorderWalkReplicaNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list);
    void inorderWalkDelete(TNode<K,V> *x);
};

template<typename K, typename V>
RBTree<K,V>::RBTree()
{
    size_ = 0;
    nil_ = new TNode<K,V>(BLACK, NULL, NULL, NULL);
    root_ = new TNode<K,V>(BLACK, nil_, nil_, nil_);
}


template<typename K, typename V>
RBTree<K,V>::RBTree(K key, V value)
{
    size_ = 0;
    nil_ = new TNode<K,V>(key, value, BLACK, NULL, NULL, NULL);
    root_ = new TNode<K,V>(key, value, BLACK, nil_, nil_, nil_);
}

template<typename K, typename V>
RBTree<K,V>::~RBTree()
{
    removeAll();
    delete nil_;
}

template<typename K, typename V>
void RBTree<K,V>::inorderWalk(TNode<K,V> *x, std::ostream_iterator<K> &iter)
{
    if (x != nil_) {
        inorderWalk(x->left(), iter);
        *iter++ = x->key();
        inorderWalk(x->right(), iter);
    }
}

template<typename K, typename V>
QList<TNode<K,V>*> RBTree<K,V>::nodes()
{
    QList<TNode<K,V>*> list;
    if(root_ != nil_) {
        inorderWalkNodesAppend(root_, list);
    }
    return list;
}

template<typename K, typename V>
void RBTree<K,V>::inorderWalkNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list)
{
    if (x != nil_) {
        inorderWalkNodesAppend(x->left(), list);
        list.append(x);
        inorderWalkNodesAppend(x->right(), list);
    }
}

template<typename K, typename V>
QList<TNode<K,V>*> RBTree<K,V>::replicaNodes()
{
    QList<TNode<K,V>*> list;
    if(root_ != nil_) {
        inorderWalkReplicaNodesAppend(root_, list);
    }
    return list;
}

template<typename K, typename V>
void RBTree<K,V>::inorderWalkReplicaNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list)
{
    if (x != nil_) {
        inorderWalkReplicaNodesAppend(x->left(), list);
        if (x->is_replica()) {
            list.append(x);
        }
        inorderWalkReplicaNodesAppend(x->right(), list);
    }
}

template<typename K, typename V>
TNode<K,V>* RBTree<K,V>::search(TNode<K,V> *x, K key)
{
    if (x == nil_ || key == x->key()) {
        return x;
    } else if (key < x->key()) {
        return search(x->left(), key);
    } else {
        return search(x->right(), key);
    }
}

template<typename K, typename V>
vector<V> RBTree<K,V>::search(K key)
{
    return search(root_, key)->values();
}

template<typename K, typename V>
TNode<K,V>& RBTree<K,V>::getNode(K key)
{
    return *search(root_, key);
}

template<typename K, typename V>
void RBTree<K,V>::leftRotate(TNode<K,V> *x)
{
    TNode<K,V>* y = x->right();
    x->set_right(y->left());
    if (y->left() != nil_) {
        y->left()->set_parent(x);
    }
    y->set_parent(x->parent());
    if (x->parent() == nil_) {
        root_ = y;
    } else if (x == x->parent()->left()) {
        x->parent()->set_left(y);
    } else {
        x->parent()->set_right(y);
    }
    y->set_left(x);
    x->set_parent(y);
}

template<typename K, typename V>
void RBTree<K,V>::rightRotate(TNode<K,V> *x)
{
    TNode<K,V>* y = x->left();
    x->set_left(y->right());
    if (y->right() != nil_) {
        y->right()->set_parent(x);
    }
    y->set_parent(x->parent());
    if (x->parent() == nil_) {
        root_ = y;
    } else if (x == x->parent()->left()) {
        x->parent()->set_left(y);
    } else {
        x->parent()->set_right(y);
    }
    y->set_right(x);
    x->set_parent(y);
}

template<typename K, typename V>
void RBTree<K,V>::insert(K key, V value)
{
    TNode<K,V> *z = new TNode<K,V>(key, value, BLACK, nil_, nil_, nil_);
    insert(z);
}

template<typename K, typename V>
void RBTree<K,V>::insert(K key, vector<V> values)
{
    TNode<K,V> *z = new TNode<K,V>(key, values, BLACK, nil_, nil_, nil_);
    insert(z);
}

template<typename K, typename V>
void RBTree<K,V>::replace(K key, vector<V> values)
{
    search(root_, key)->set_values(values);
}

template<typename K, typename V>
void RBTree<K,V>::insert(TNode<K,V> *z)
{
    size_++;
    TNode<K,V> *y = nil_;
    TNode<K,V> *x = root_;
    while (x != nil_) {
        y = x;
        if (z->key() == x->key()) {
            x->addValues(z->values());
            delete z;
            return;
        } else if (z->key() < x->key()) {
            x = x->left();
        } else {
            x = x->right();
        }
    }
    z->set_parent(y);
    if (y == nil_) {
        root_ = z;
    } else if (z->key() < y->key()) {
        y->set_left(z);
    } else {
        y->set_right(z);
    }
    z->set_left(nil_);
    z->set_right(nil_);
    z->set_color(RED);
    insertFixup(z);
}

template<typename K, typename V>
void RBTree<K,V>::insertFixup(TNode<K,V> *z)
{
    while (z->parent()->color() == RED) {
        if (z->parent() == z->parent()->parent()->left()) {
            TNode<K,V>* y = z->parent()->parent()->right();
            if (y->color() == RED) {
                z->parent()->set_color(BLACK);
                y->set_color(BLACK);
                z->parent()->parent()->set_color(RED);
                z = z->parent()->parent();
            } else {
                if (z == z->parent()->right()) {
                    z = z->parent();
                    leftRotate(z);
                }
                z->parent()->set_color(BLACK);
                z->parent()->parent()->set_color(RED);
                rightRotate(z->parent()->parent());
            }
        } else {
            TNode<K,V>* y = z->parent()->parent()->left();
            if (y->color() == RED) {
                z->parent()->set_color(BLACK);
                y->set_color(BLACK);
                z->parent()->parent()->set_color(RED);
                z = z->parent()->parent();
            } else {
                if (z == z->parent()->left()) {
                    z = z->parent();
                    rightRotate(z);
                }
                z->parent()->set_color(BLACK);
                z->parent()->parent()->set_color(RED);
                leftRotate(z->parent()->parent());
            }
        }
    }
    root_->set_color(BLACK);
}

template<typename K, typename V>
void RBTree<K,V>::removeAll()
{
    inorderWalkDelete(root_);
}

template<typename K, typename V>
void RBTree<K,V>::inorderWalkDelete(TNode<K,V> *x)
{
    if (x != nil_) {
        inorderWalkDelete(x->left());
        delete remove(x);
        inorderWalkDelete(x->right());
    }
}

template<typename K, typename V>
void RBTree<K,V>::remove(K key)
{
    delete remove(search(root_, key));
}

template<typename K, typename V>
TNode<K,V>* RBTree<K,V>::remove(TNode<K,V> *z) // don't delete memory
{
    size_--;
    TNode<K,V> *y = nil_;
    if (z->left() == nil_ || z->right() == nil_) {
        y = z;
    } else {
        y = successor(z);
    }
    TNode<K,V> *x = nil_;
    if (y->left() != nil_) {
        x = y->left();
    } else {
        x = y->right();
    }
    x->set_parent(y->parent());
    if (y->parent() == nil_) {
        root_ = x;
    } else if (y == y->parent()->left()) {
        y->parent()->set_left(x);
    } else {
        y->parent()->set_right(x);
    }
    if (y != z) {
        z->set_key(y->key());
        z->set_values(y->values());
        z->set_color(y->color());
        z->set_left(y->left());
        z->set_right(y->right());
        z->set_parent(y->parent());
    }
    if (y->color() == BLACK) {
        removeFixup(x);
    }
    return y;
}

template<typename K, typename V>
TNode<K,V>* RBTree<K,V>::successor(TNode<K,V> *x)
{
    if (x->right() != nil_) {
        return minimum(x->right());
    }
    TNode<K,V> *y = x->parent();
    while (y != nil_ && x == y->right()) {
        x = y;
        y = y->parent();
    }
    return y;
}

template<typename K, typename V>
TNode<K,V>* RBTree<K,V>::minimum(TNode<K,V> *x)
{
    while (x->left() != nil_) {
        x = x->left();
    }
    return x;
}

template<typename K, typename V>
void RBTree<K,V>::removeFixup(TNode<K,V> *x)
{
    while (x != root_ && x->color() == BLACK) {
        if (x == x->parent()->left()) {
            TNode<K,V> *w = x->parent()->right();
            if (x->color() == RED) {
                w->set_color(BLACK);
                x->parent()->set_color(RED);
                leftRotate(x->parent());
                w = x->parent()->right();
            }
            if (w->left()->color() == BLACK && w->right()->color() == BLACK) {
                w->set_color(RED);
                x = x->parent();
            } else if (w->right()->color() == BLACK) {
                w->left()->set_color(BLACK);
                w->set_color(RED);
                rightRotate(w);
                w = x->parent()->right();
            } else {
                w->set_color(x->parent()->color());
                x->parent()->set_color(BLACK);
                w->right()->set_color(BLACK);
                leftRotate(x->parent());
                x = root_;
            }
        } else {
            TNode<K,V> *w = x->parent()->left();
            if (x->color() == RED) {
                w->set_color(BLACK);
                x->parent()->set_color(RED);
                rightRotate(x->parent());
                w = x->parent()->left();
            }
            if (w->right()->color() == BLACK && w->left()->color() == BLACK) {
                w->set_color(RED);
                x = x->parent();
            } else if (w->left()->color() == BLACK) {
                w->right()->set_color(BLACK);
                w->set_color(RED);
                leftRotate(w);
                w = x->parent()->left();
            } else {
                w->set_color(x->parent()->color());
                x->parent()->set_color(BLACK);
                w->left()->set_color(BLACK);
                rightRotate(x->parent());
                x = root_;
            }
        }
    }
}

#endif // RBTREE_H
