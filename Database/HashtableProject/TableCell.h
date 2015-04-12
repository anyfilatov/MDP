#include <string>
#include "stdafx.h"
#include <sstream>
#include <conio.h>
#include <vector>

using namespace std;

template <typename V>
class TableCell{

private:

	int hash;
	int size;
	CellItem<V>* listRoot;


public:
	
	TableCell(void);
	TableCell(int hash);
	int getHash();
	void setHash(int hash);
	bool add(CellItem<V>* item);
	V get(K key);
	void update(Hashable key, V value);
	CellItem<V>* remove(Hashable key);
	int getSize();
	string toString(); 
	vector<Hashable> keys();
};

template <typename V>
TableCell<V>::TableCell(void){
	hash = -1;
	size = 0;
	listRoot = NULL;
}

template <typename V>
TableCell<V>::TableCell(int hash){
	this->hash = hash;
	size = 0;
	listRoot = NULL;
}

template <typename V>
int TableCell<V>::getHash(){
	return hash;
}

template <typename V>
void TableCell<V>::setHash(int hash){
	this->hash = hash;
}

template <typename V>
bool TableCell<V>::add(CellItem<V>* item){
	if (!item) return false;
	if ((!listRoot) || (size == 0)){
		listRoot = item;
		size = 1;
		return true;
	}
	CellItem<V> *current, *prev;
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

template <typename V>
V TableCell<V>::get(K key){
	CellItem<V> *current;
	current = listRoot;
	while (current){
		if (current->getKey() == key)
			return current->getValue();
		current = current->getNext(); 
	}
	return NULL;
}

template <typename V>
void TableCell<V>::update(K key, V value){
	CellItem<V> *current;
	current = listRoot;
	while (current){
		if (current->getKey() == key){
			current->setValue(value);
			break;
		}
		current = current->getNext(); 
	}
}

template <typename V>
CellItem<V>* TableCell<V>::remove(K key){
	CellItem<V> *current, *prev;
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

template <typename V>
int TableCell<V>::getSize(){
	return size;
}

template <typename V>
string TableCell<V>::toString(){
	stringstream ss;
	ss << "TableCell\n{\n  hash: " << hash << "\n  size: " << size << "\n}\n";
	return ss.str();
}

template <typename V>
vector<K> TableCell<V>::keys(){
	vector<K> keys(size);
	CellItem<V> *current = listRoot;
	for (int i = 0; i < size; i++){
		if (!current) break;
		keys[i] = current->getKey();
		current = current->getNext();
	}
	return keys;
}