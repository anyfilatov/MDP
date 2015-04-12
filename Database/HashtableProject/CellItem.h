#include <string>
#include "stdafx.h"
#include <sstream>

using namespace std;

template <typename V>
class CellItem{

private:

	Hashable key;
	V value;
	CellItem<V>* next;

public:

	CellItem();
	CellItem(Hashable key, V value);
	void setKey(Hashable key);
	Hashable getKey();
	void setValue(V value);
	V getValue();
	CellItem<V>* getNext();
	void setNext(CellItem<V>* item);
	string toString();

};

template <typename V>
CellItem<V>::CellItem(){
	next = NULL;
}

template <typename V>
CellItem<V>::CellItem(Hashable key, V value){
	this->key = key;
	this->value = value;
	next = NULL;
}

template <typename V>
void CellItem<V>::setKey(Hashable key){
	this->key = key;
}

template <typename V>
Hashable  CellItem<V>::getKey(){
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
	ss << "CellItem\n{\n  key: " << key << "\n  value: " << value << "\n}\n";
	return ss.str();
}