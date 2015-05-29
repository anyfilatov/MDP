#ifndef CellItem_h
#define CellItem_h

#include <sstream>
#include <QTextStream>
#include "HashTable/AbstractTableKey.h"

using namespace std;

namespace database{

template <typename V>
class CellItem{

private:

    AbstractTableKey*  key;
    V* value;
	CellItem<V>* next;

public:

	CellItem();
    CellItem(AbstractTableKey* key, V* value);
    void setKey(AbstractTableKey*  key);
    AbstractTableKey*  getKey();
    void setValue(V* value);
    V* getValue();
	CellItem<V>* getNext();
	void setNext(CellItem<V>* item);
	string toString();
    void clear();

};

template <typename V>
CellItem<V>::CellItem(){
	next = NULL;
}

template <typename V>
CellItem<V>::CellItem(AbstractTableKey*  key, V* value){
	this->key = key;
	this->value = value;
	next = NULL;
}

template <typename V>
void CellItem<V>::setKey(AbstractTableKey*  key){
	this->key = key;
}

template <typename V>
AbstractTableKey* CellItem<V>::getKey(){
	return key;
}

template <typename V>
void CellItem<V>::setValue(V* value){
	this->value = value;
}

template <typename V>
V* CellItem<V>::getValue(){
	return value;
}

template <typename V>
CellItem<V>* CellItem<V>::getNext(){
	return next;
}

template <typename V>
void CellItem<V>::setNext(CellItem<V>* item){
	next = item;
}

template <typename V>
string CellItem<V>::toString(){
	stringstream ss;
    ss << "CellItem\n{\n  key Hash: " << key->hash() << "\n}\n";
	return ss.str();
}

template <typename V>
void CellItem<V>::clear(){
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

