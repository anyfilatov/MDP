#ifndef TableCell_h
#define TableCell_h

#include <string>
#include <sstream>
#include <conio.h>
#include <vector>
#include <QTextStream>
#include "CellItem.h"
#include "AbstractTableKey.h"

using namespace std;

template <typename V>
class TableCell{

private:

	int hash;
    int size;


public:

    CellItem<V>* listRoot;
	TableCell(void);
	TableCell(int hash);
	int getHash();
	void setHash(int hash);
	bool add(CellItem<V>* item);
    V* get(AbstractTableKey* key);
    void update(AbstractTableKey* key, V* value);
    CellItem<V>* remove(AbstractTableKey* key);
	int getSize();
	string toString(); 
    vector<AbstractTableKey*> keys();
    vector<pair<AbstractTableKey*, V> > entries();
    void clear();
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
        if (current->getKey()->equals(item->getKey()))
			return false;
		prev = current;
		current = current->getNext(); 
	}
	prev->setNext(item);
	size++;
	return true;
}

template <typename V>
V* TableCell<V>::get(AbstractTableKey* key){
	CellItem<V> *current;
	current = listRoot;
	while (current){
        if (current->getKey()->equals(key))
			return current->getValue();
		current = current->getNext(); 
	}
	return NULL;
}

template <typename V>
void TableCell<V>::update(AbstractTableKey* key, V* value){
	CellItem<V> *current;
	current = listRoot;
	while (current){
        if (current->getKey()->equals(key)){
            V* oldVal = current->getValue();
			current->setValue(value);
            delete oldVal;
			break;
		}
		current = current->getNext(); 
	}
}

template <typename V>
CellItem<V>* TableCell<V>::remove(AbstractTableKey* key){
	CellItem<V> *current, *prev;
    if (listRoot == NULL){
        return NULL;
    }
	prev = listRoot;
    if (listRoot->getKey()->equals(key)){
		current = listRoot->getNext();
		prev->setNext(NULL);
		listRoot = current;
        size--;
		return prev;
	}
	current = listRoot->getNext();
	while (current){
        if (current->getKey()->equals(key)){
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
vector<AbstractTableKey*> TableCell<V>::keys(){
    vector<AbstractTableKey*  > keys(size);
	CellItem<V> *current = listRoot;
	for (int i = 0; i < size; i++){
		if (!current) break;
		keys[i] = current->getKey();
		current = current->getNext();
	}
	return keys;
}

template <typename V>
vector<pair<AbstractTableKey*, V> > TableCell<V>::entries(){
    vector<pair<AbstractTableKey*, V> > entries(size);
    CellItem<V> *current = listRoot;
    for (int i = 0; i < size; i++){
        if (!current) break;
        entries[i] = pair<AbstractTableKey*, V>((AbstractTableKey*) current->getKey()->clone(), *current->getValue());
        current = current->getNext();
    }
    return entries;
}

template <typename V>
void TableCell<V>::clear(){
    if (listRoot != NULL){
        listRoot->clear();
    }
    delete this;
}

#endif
