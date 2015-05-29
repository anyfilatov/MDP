#ifndef RBTREE_H
#define RBTREE_H

#include "tnode.h"
#include <iostream>
#include <vector>
#include <QList>
#include <cmath>
#include <memory>

using namespace std;

template<typename K, typename V, class Alloc = std::allocator<TNode<K, V> > >
class RBTree
{
public:
    RBTree();
    ~RBTree();

    void insert(TNode<K,V> *z, bool rewrite = false);
    void insert(K key, V value, bool rewrite = false, bool replica = false);
    void insert(K key, vector<V> values, bool replica = false);
    void replace(K key, vector<V> values);

    void remove(K key);
    void remove(K key, V value);
    TNode<K,V>* remove(TNode<K,V> *z); // don't delete memory
    void removeAll();

    TNode<K,V>* search(TNode<K,V> *x, K key);
    TNode<K,V>& getNode(K key);
    vector<V> search(K key);

    void inorderWalk(TNode<K,V> *x, std::ostream_iterator<K> &iter);
    QList<K> getKeys();
    QList<TNode<K,V>*> nodes();
    QList<TNode<K,V>*> primaryNodes();
    QList<TNode<K,V>*> replicaNodes();

    // For GUI
    unsigned int redNodesCount();
    unsigned int blackNodesCount();
    unsigned int primaryNodesCount();
    unsigned int replicaNodesCount();
    unsigned int treeHeight();
    unsigned int valuesCount();

    bool isEmpty() { return size_ == 0 ? true : false; }
    unsigned int size() { return size_; }

    TNode<K,V>* root() { return root_; }

private:
    TNode<K,V> *root_;
    TNode<K,V> *nil_;
    unsigned int size_;
    Alloc* allocator;

    void leftRotate(TNode<K,V> *x);
    void rightRotate(TNode<K,V> *x);
    void insertFixup(TNode<K,V> *z);
    TNode<K,V>* successor(TNode<K,V> *x);
    TNode<K,V>* minimum(TNode<K,V> *x);
    void removeFixup(TNode<K,V> *x);
    void inorderWalkKeysAppend(TNode<K,V> *x, QList<K>& list);
    void inorderWalkNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list);
    void inorderWalkPrimaryNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list);
    void inorderWalkReplicaNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list);
    void inorderWalkDelete(TNode<K,V> *x);
    void inorderWalkRedNodesCount(TNode<K,V> *x, unsigned int& count);
    void inorderWalkValuesCount(TNode<K,V> *x, unsigned int& count);
};

template<typename K, typename V, class Alloc>
RBTree<K,V, Alloc>::RBTree()
{
    allocator = new Alloc();
    size_ = 0;
    nil_ = allocator->allocate(1);
    allocator->construct(nil_, TNode<K,V>(BLACK, NULL, NULL, NULL));
//    nil_ = new TNode<K,V>(BLACK, NULL, NULL, NULL);
    root_ = nil_;
}

template<typename K, typename V, class Alloc>
RBTree<K,V, Alloc>::~RBTree()
{
    removeAll();
    allocator->destroy(nil_);
    allocator->deallocate(nil_, 1);
//    delete nil_;
    delete allocator;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V, Alloc>::inorderWalk(TNode<K,V> *x, std::ostream_iterator<K> &iter)
{
    if (x != nil_) {
        inorderWalk(x->left(), iter);
        *iter++ = x->key();
        inorderWalk(x->right(), iter);
    }
}

template<typename K, typename V, class Alloc>
QList<K> RBTree<K,V, Alloc>::getKeys()
{
    QList<K> list;
    if (root_ != nil_) {
        inorderWalkKeysAppend(root_, list);
    }
    return list;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V, Alloc>::inorderWalkKeysAppend(TNode<K,V> *x, QList<K>& list)
{
    if (x != nil_) {
        inorderWalkKeysAppend(x->left(), list);
        list.append(x->key());
        inorderWalkKeysAppend(x->right(), list);
    }
}

template<typename K, typename V, class Alloc>
QList<TNode<K,V>*> RBTree<K,V,Alloc>::nodes()
{
    QList<TNode<K,V>*> list;
    if(root_ != nil_) {
        inorderWalkNodesAppend(root_, list);
    }
    return list;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list)
{
    if (x != nil_) {
        inorderWalkNodesAppend(x->left(), list);
        list.append(x);
        inorderWalkNodesAppend(x->right(), list);
    }
}

template<typename K, typename V, class Alloc>
QList<TNode<K,V>*> RBTree<K,V,Alloc>::primaryNodes()
{
    QList<TNode<K,V>*> list;
    if(root_ != nil_) {
        inorderWalkPrimaryNodesAppend(root_, list);
    }
    return list;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkPrimaryNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list)
{
    if (x != nil_) {
        inorderWalkPrimaryNodesAppend(x->left(), list);
        if (!x->is_replica()) {
            list.append(x);
        }
        inorderWalkPrimaryNodesAppend(x->right(), list);
    }
}

template<typename K, typename V, class Alloc>
QList<TNode<K,V>*> RBTree<K,V,Alloc>::replicaNodes()
{
    QList<TNode<K,V>*> list;
    if (root_ != nil_) {
        inorderWalkReplicaNodesAppend(root_, list);
    }
    return list;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkReplicaNodesAppend(TNode<K,V> *x, QList<TNode<K,V>*>& list)
{
    if (x != nil_) {
        inorderWalkReplicaNodesAppend(x->left(), list);
        if (x->is_replica()) {
            list.append(x);
        }
        inorderWalkReplicaNodesAppend(x->right(), list);
    }
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::redNodesCount()
{
    unsigned int count = 0;
    if (root_ != nil_) {
        inorderWalkRedNodesCount(root_, count);
    }
    return count;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkRedNodesCount(TNode<K,V> *x, unsigned int& count)
{
    if (x != nil_) {
        inorderWalkRedNodesCount(x->left(), count);
        if (x->color() == RED) {
            count++;
        }
        inorderWalkRedNodesCount(x->right(), count);
    }
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::blackNodesCount()
{
    return size_ - redNodesCount();
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::primaryNodesCount()
{
    return primaryNodes().size();
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::replicaNodesCount()
{
    return replicaNodes().size();
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::treeHeight()
{
    unsigned int height = 0;
    int count = size_;
    while (count > 0) {
        count -= pow(2, height);
        height++;
    }
    return height;
}

template<typename K, typename V, class Alloc>
unsigned int RBTree<K,V,Alloc>::valuesCount()
{
    unsigned int count = 0;
    if (root_ != nil_) {
        inorderWalkValuesCount(root_, count);
    }
    return count;
}


template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkValuesCount(TNode<K,V> *x, unsigned int& count)
{
    if (x != nil_) {
        inorderWalkValuesCount(x->left(), count);
        count += x->values().size();
        inorderWalkValuesCount(x->right(), count);
    }
}

template<typename K, typename V, class Alloc>
TNode<K,V>* RBTree<K,V,Alloc>::search(TNode<K,V> *x, K key)
{
    if (x == nil_ || key == x->key()) {
        return x;
    } else if (key < x->key()) {
        return search(x->left(), key);
    } else {
        return search(x->right(), key);
    }
}

template<typename K, typename V, class Alloc>
vector<V> RBTree<K,V,Alloc>::search(K key)
{
    return search(root_, key)->values();
}

template<typename K, typename V, class Alloc>
TNode<K,V>& RBTree<K,V,Alloc>::getNode(K key)
{
    return *search(root_, key);
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::leftRotate(TNode<K,V> *x)
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

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::rightRotate(TNode<K,V> *x)
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

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::insert(K key, V value, bool rewrite, bool replica)
{
    TNode<K,V> *z = allocator->allocate(1);
    allocator->construct(z, TNode<K,V>(key, value, BLACK, nil_, nil_, nil_));
//    TNode<K,V> *z = new TNode<K,V>(key, value, BLACK, nil_, nil_, nil_);
    z->set_replica(replica);
    insert(z, rewrite);
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::insert(K key, vector<V> values, bool replica)
{
    TNode<K,V> *z = allocator->allocate(1);
    allocator->construct(z, TNode<K,V>(key, values, BLACK, nil_, nil_, nil_));
//    TNode<K,V> *z = new TNode<K,V>(key, values, BLACK, nil_, nil_, nil_);
    z->set_replica(replica);
    insert(z);
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::replace(K key, vector<V> values)
{
    search(root_, key)->set_values(values);
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::insert(TNode<K,V> *z, bool rewrite)
{
    size_++;
    if (root_ == nil_) {
        root_ = z;
        return;
    }
    TNode<K,V> *y = nil_;
    TNode<K,V> *x = root_;
    while (x != nil_) {
        y = x;
        if (z->key() == x->key()) {
            x->addValues(z->values(), rewrite);
            size_--;
            allocator->destroy(z);
            allocator->deallocate(z, 1);
//            delete z;
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

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::insertFixup(TNode<K,V> *z)
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

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::removeAll()
{
    inorderWalkDelete(root_);
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::inorderWalkDelete(TNode<K,V> *x)
{
    if (x != nil_) {
        inorderWalkDelete(x->left());
        TNode<K,V>* removed = remove(x);
        allocator->destroy(removed);
        allocator->deallocate(removed, 1);
//        delete remove(x);
        inorderWalkDelete(x->right());
    }
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::remove(K key)
{
    if (search(root_, key) != nil_) {
        TNode<K,V>* removed = remove(search(root_, key));
        allocator->destroy(removed);
        allocator->deallocate(removed, 1);
//        delete remove(search(root_, key));
    }
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::remove(K key, V value)
{
    TNode<K,V>* node = search(root_, key);
    if (node != nil_) {
        node->removeValue(value);
        if (node->values().size() == 0) {
            TNode<K,V>* removed = remove(node);
            allocator->destroy(removed);
            allocator->deallocate(removed, 1);
//            delete remove(node);
        }
    }
}

template<typename K, typename V, class Alloc>
TNode<K,V>* RBTree<K,V,Alloc>::remove(TNode<K,V> *z) // don't delete memory
{
//    cout << "root.k = " << root_->key() << endl;
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

template<typename K, typename V, class Alloc>
TNode<K,V>* RBTree<K,V,Alloc>::successor(TNode<K,V> *x)
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

template<typename K, typename V, class Alloc>
TNode<K,V>* RBTree<K,V,Alloc>::minimum(TNode<K,V> *x)
{
    while (x->left() != nil_) {
        x = x->left();
    }
    return x;
}

template<typename K, typename V, class Alloc>
void RBTree<K,V,Alloc>::removeFixup(TNode<K,V> *x)
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
