#ifndef TableCell_h
#define TableCell_h

#include <string>
#include <sstream>
#include <conio.h>
#include <vector>
#include <QTextStream>
#include "CellItem.h"
#include "TableKey.h"

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
    V get(TableKey* key);
    void update(TableKey* key, V value);
    CellItem<V>* remove(TableKey* key);
	int getSize();
	string toString(); 
    vector<TableKey*> keys();
    vector<pair<TableKey*, V> > entries();
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
        if (current->getKey()->hash() == item->getKey()->hash())
			return false;
		prev = current;
		current = current->getNext(); 
	}
	prev->setNext(item);
	size++;
	return true;
}

template <typename V>
V TableCell<V>::get(TableKey* key){
	CellItem<V> *current;
	current = listRoot;
	while (current){
        if (current->getKey()->hash() == key->hash())
			return current->getValue();
		current = current->getNext(); 
	}
	return NULL;
}

template <typename V>
void TableCell<V>::update(TableKey* key, V value){
	CellItem<V> *current;
	current = listRoot;
	while (current){
        if (current->getKey()->hash() == key->hash()){
			current->setValue(value);
			break;
		}
		current = current->getNext(); 
	}
}

template <typename V>
CellItem<V>* TableCell<V>::remove(TableKey* key){
    QTextStream cout(stdout);
    cout << "REMOVE FROM CELL\n root: ";
	CellItem<V> *current, *prev;
    if (listRoot == NULL){
        if (listRoot == NULL)
            cout << "NULL\n";
        else cout << "NOT NULL\n";
        return NULL;
    }
	prev = listRoot;
    if (listRoot->getKey() == key){
		current = listRoot->getNext();
		prev->setNext(NULL);
		listRoot = current;
        size--;
        if (listRoot == NULL)
            cout << "NULL\n";
        else cout << "NOT NULL\n";
		return prev;
	}
	current = listRoot->getNext();
	while (current){
		if (current->getKey() == key){
			prev->setNext(current->getNext());
			current->setNext(NULL);
            size--;
            if (listRoot == NULL)
                cout << "NULL\n";
            else cout << "NOT NULL\n";
			return current;
		}
		prev = current;
		current = current->getNext(); 
    }
    if (listRoot == NULL)
        cout << "NULL\n";
    else cout << "NOT NULL\n";
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
vector<TableKey*> TableCell<V>::keys(){
    vector<TableKey*  > keys(size);
	CellItem<V> *current = listRoot;
	for (int i = 0; i < size; i++){
		if (!current) break;
		keys[i] = current->getKey();
		current = current->getNext();
	}
	return keys;
}

template <typename V>
vector<pair<TableKey*, V> > TableCell<V>::entries(){
    vector<pair<TableKey*, V> > entries(size);
    CellItem<V> *current = listRoot;
    for (int i = 0; i < size; i++){
        if (!current) break;
        entries[i] = pair<TableKey*, V>((TableKey*) current->getKey()->clone(), current->getValue());
        current = current->getNext();
    }
    return entries;
}

template <typename V>
void TableCell<V>::clear(){
    QTextStream cout(stdout);
    cout << "CLEAR Cell\n";
    if (listRoot != NULL){
        cout << listRoot->getKey()->hash() << endl;
        listRoot->clear();
    }
    else
        cout << " NULL\n";
    delete this;
}

#endif
