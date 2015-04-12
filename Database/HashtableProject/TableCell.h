#include <string>
#include "stdafx.h"
#include <sstream>
#include <conio.h>
#include <vector>

using namespace std;

template <typename K, typename V>
class TableCell{

private:

	int hash;
	int size;
	CellItem<K, V>* listRoot;


public:
	
	TableCell(void);
	TableCell(int hash);
	int getHash();
	void setHash(int hash);
	bool add(CellItem<K,V>* item);
	V get(K key);
	void update(K key, V value);
	CellItem<K,V>* remove(K key);
	int getSize();
	string toString(); 
	vector<K> keys();
};

template <typename K, typename V>
TableCell<K,V>::TableCell(void){
	hash = -1;
	size = 0;
	listRoot = NULL;
}

template <typename K, typename V>
TableCell<K,V>::TableCell(int hash){
	this->hash = hash;
	size = 0;
	listRoot = NULL;
}

template <typename K, typename V>
int TableCell<K,V>::getHash(){
	return hash;
}

template <typename K, typename V>
void TableCell<K,V>::setHash(int hash){
	this->hash = hash;
}

template <typename K, typename V>
bool TableCell<K,V>::add(CellItem<K,V>* item){
	if (!item) return false;
	if ((!listRoot) || (size == 0)){
		listRoot = item;
		size = 1;
		return true;
	}
	CellItem<K,V> *current, *prev;
	current = listRoot;
	while (current){
		if (current->getKey() == item->getKey())
			return false;
		prev = current;
		current = current->getNext(); 
	}
	prev->setNext(item);
	size++;
	return true;
}

template <typename K, typename V>
V TableCell<K,V>::get(K key){
	CellItem<K,V> *current;
	current = listRoot;
	while (current){
		if (current->getKey() == key)
			return current->getValue();
		current = current->getNext(); 
	}
	return NULL;
}

template <typename K, typename V>
void TableCell<K,V>::update(K key, V value){
	CellItem<K,V> *current;
	current = listRoot;
	while (current){
		if (current->getKey() == key){
			current->setValue(value);
			break;
		}
		current = current->getNext(); 
	}
}

template <typename K, typename V>
CellItem<K,V>* TableCell<K,V>::remove(K key){
	CellItem<K,V> *current, *prev;
	if (listRoot->getKey() == key){
		current = listRoot;
		current->setNext(NULL);
		listRoot = listRoot->getNext();
		size--;
		return current;
	}
	prev = listRoot;
	current = listRoot->getNext();
	while (current){
		if (current->getKey() == key){
			prev->setNext(current->getNext());
			current->setNext(NULL);
			size--;
			return current;
		}
		prev = current;
		current = current->getNext(); 
	}
	return NULL;
}

template <typename K, typename V>
int TableCell<K,V>::getSize(){
	return size;
}

template <typename K, typename V>
string TableCell<K,V>::toString(){
	stringstream ss;
	ss << "TableCell\n{\n  hash: " << hash << "\n  size: " << size << "\n}\n";
	return ss.str();
}

template <typename K, typename V>
vector<K> TableCell<K,V>::keys(){
	vector<K> keys(size);
	CellItem<K,V> *current = listRoot;
	for (int i = 0; i < size; i++){
		if (!current) break;
		keys[i] = current->getKey();
		current = current->getNext();
	}
	return keys;
}