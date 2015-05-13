#ifndef QRBNODE_H
#define QRBNODE_H

#include <vector>

using namespace std;

template<typename V>
class QRBNode
{
public:
    QRBNode() {}
    QRBNode(V value);
    QRBNode(V value, bool replica);
    QRBNode(vector<V> values);
    QRBNode(vector<V> values, bool replica);
    ~QRBNode() {}

    void addValue(V value);
    void addValues(vector<V> values);

    vector<V> values() const { return values_; }
    bool is_replica() const { return replica_; }

    void set_values(vector<V> values) { values_ = values; }
    void set_replica(bool replica) { replica_ = replica; }

private:
    vector<V> values_;
    bool replica_;
};

template<typename V>
QRBNode<V>::QRBNode(V value)
{
    values_.push_back(value);
    replica_ = false;
}

template<typename V>
QRBNode<V>::QRBNode(V value, bool replica)
{
    values_.push_back(value);
    replica_ = replica;
}

template<typename V>
QRBNode<V>::QRBNode(vector<V> values)
{
    values_ = values;
    replica_ = false;
}

template<typename V>
QRBNode<V>::QRBNode(vector<V> values, bool replica)
{
    values_ = values;
    replica_ = replica;
}

template<typename V>
void QRBNode<V>::addValue(V value)
{
    values_.push_back(value);
}

template<typename V>
void QRBNode<V>::addValues(vector<V> values)
{
    for (V value : values) {
        values_.push_back(value);
    }
}

#endif // QRBNODE_H
