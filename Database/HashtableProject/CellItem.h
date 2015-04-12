#include <string>
#include "stdafx.h"
#include <sstream>

using namespace std;

template <typename K, typename V>
class CellItem{

private:

	K key;
	V value;
	CellItem<K,V>* next;

public:

	CellItem();
	CellItem(K key, V value);
	void setKey(K key);
	K getKey();
	void setValue(V value);
	V getValue();
	CellItem<K,V>* getNext();
	void setNext(CellItem<K,V>* item);
	string toString();

};

template <typename K, typename V>
CellItem<K,V>::CellItem(){
	next = NULL;
}

template <typename K, typename V>
CellItem<K,V>::CellItem(K key, V value){
	this->key = key;
	this->value = value;
	next = NULL;
}

template <typename K, typename V>
void CellItem<K,V>::setKey(K key){
	this->key = key;
}

template <typename K, typename V>
K CellItem<K,V>::getKey(){
	return key;
}

template <typename K, typename V>
void CellItem<K,V>::setValue(V value){
	this->value = value;
}

template <typename K, typename V>
V CellItem<K,V>::getValue(){
	return value;
}

template <typename K, typename V>
CellItem<K,V>* CellItem<K,V>::getNext(){
	return next;
}

template <typename K, typename V>
void CellItem<K,V>::setNext(CellItem<K,V>* item){
	next = item;
}

template <typename K, typename V>
string CellItem<K,V>::toString(){
	stringstream ss;
	ss << "CellItem\n{\n  key: " << key << "\n  value: " << value << "\n}\n";
	return ss.str();
}