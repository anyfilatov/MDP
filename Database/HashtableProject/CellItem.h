#ifndef CellItem_h
#define CellItem_h

#include <sstream>
#include <QTextStream>
#include "TableKey.h"

using namespace std;

template <typename V>
class CellItem{

private:

    TableKey*  key;
	V value;
	CellItem<V>* next;

public:

	CellItem();
    CellItem(TableKey* key, V value);
    void setKey(TableKey*  key);
    TableKey*  getKey();
	void setValue(V value);
	V getValue();
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
CellItem<V>::CellItem(TableKey*  key, V value){
	this->key = key;
	this->value = value;
	next = NULL;
}

template <typename V>
void CellItem<V>::setKey(TableKey*  key){
	this->key = key;
}

template <typename V>
TableKey* CellItem<V>::getKey(){
	return key;
}

template <typename V>
void CellItem<V>::setValue(V value){
	this->value = value;
}

template <typename V>
V CellItem<V>::getValue(){
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
	ss << "CellItem\n{\n  key Hash: " << key->hash() << "\n  value: " << value << "\n}\n";
	return ss.str();
}

template <typename V>
void CellItem<V>::clear(){
    QTextStream cout(stdout);
    cout << " CLEAR cellItem\n";
    cout << "  delete key\n";
    if (key != NULL){
        cout << "   key: " << key->hash() << endl;
        //delete key;
    }
    cout << "  delete value\n";
    delete &value;
    cout << "  delete next\n";
    if (next != NULL){
        next->getValue();
        cout << "  next: " << next->getKey()->hash() << endl;
        next->clear();
    }
    else
        cout << "  next: NULL\n";

    delete this;
}

#endif

