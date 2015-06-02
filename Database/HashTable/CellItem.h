#ifndef CellItem_h
#define CellItem_h

#include <sstream>
#include <QTextStream>

using namespace std;

namespace database{

template <typename K, typename V>
class CellItem{

private:

    K*  key;
    V* value;
    CellItem<K, V>* next;

public:

	CellItem();
    CellItem(K* key, V* value);
    void setKey(K* key);
    K* getKey();
    void setValue(V* value);
    V* getValue();
    CellItem<K, V>* getNext();
    void setNext(CellItem<K, V>* item);
    void clear();

};

template <typename K, typename V>
CellItem<K, V>::CellItem(){
    key = NULL;
    value = NULL;
	next = NULL;
}

template <typename K, typename V>
CellItem<K, V>::CellItem(K* key, V* value){
	this->key = key;
	this->value = value;
	next = NULL;
}

template <typename K, typename V>
void CellItem<K, V>::setKey(K* key){
	this->key = key;
}

template <typename K, typename V>
K* CellItem<K, V>::getKey(){
	return key;
}

template <typename K, typename V>
void CellItem<K, V>::setValue(V* value){
	this->value = value;
}

template <typename K, typename V>
V* CellItem<K, V>::getValue(){
	return value;
}

template <typename K, typename V>
CellItem<K, V>* CellItem<K, V>::getNext(){
	return next;
}

template <typename K, typename V>
void CellItem<K, V>::setNext(CellItem<K, V>* item){
	next = item;
}

template <typename K, typename V>
void CellItem<K, V>::clear(){
    if (key != NULL){
        delete key;
        key = NULL;
    }
    if (value != NULL){
        delete value;
        value = NULL;
    }
    if (next != NULL){
        next->clear();
        delete next;
        next = NULL;

    }
}

}

#endif

