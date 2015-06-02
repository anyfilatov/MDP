#ifndef TableCell_h
#define TableCell_h

#include <QString>
#include <sstream>
#include <vector>
#include <QTextStream>
#include "HashTable/CellItem.h"
#include "HashTable/AbstractTableKey.h"
#include "Exceptions/NotUniqueKeyException.h"
#include "Exceptions/NullPointerException.h"

using namespace std;

namespace database{

template <typename K, typename V, typename K_comparator>
class TableCell{

private:

	int hash;
    int size;


public:

    CellItem<K, V>* listRoot;
	TableCell(void);
	TableCell(int hash);
	int getHash();
	void setHash(int hash);
    bool add(CellItem<K, V>* item);
    V* get(K& key) const;
    CellItem<K, V>* remove(K& key);
    int getSize() const;
    QString toString();
    vector<K*> keys();
    vector<pair<K*, V* > > entries();
    void clear();
};

template <typename K, typename V, typename K_comparator>
TableCell<K, V, K_comparator>::TableCell(void){
	hash = -1;
	size = 0;
	listRoot = NULL;
}

template <typename K, typename V, typename K_comparator>
TableCell<K, V, K_comparator>::TableCell(int hash){
	this->hash = hash;
	size = 0;
	listRoot = NULL;
}

template <typename K, typename V, typename K_comparator>
int TableCell<K, V, K_comparator>::getHash(){
	return hash;
}

template <typename K, typename V, typename K_comparator>
void TableCell<K, V, K_comparator>::setHash(int hash){
	this->hash = hash;
}

template <typename K, typename V, typename K_comparator>
bool TableCell<K, V, K_comparator>::add(CellItem<K, V>* item){
	if (!item) return false;
	if ((!listRoot) || (size == 0)){
		listRoot = item;
		size = 1;
		return true;
	}
    CellItem<K, V> *current, *prev;
	current = listRoot;
    K_comparator Kcomp;
	while (current){
        if (Kcomp(*current->getKey(), *item->getKey())){
            return false;
        }
		prev = current;
		current = current->getNext(); 
	}
	prev->setNext(item);
	size++;
	return true;
}

template <typename K, typename V, typename K_comparator>
V* TableCell<K, V, K_comparator>::get(K& key) const{
    CellItem<K, V> *current;
	current = listRoot;
    K_comparator Kcomp;
    while (current){
        if (Kcomp(*current->getKey(), key))
			return current->getValue();
		current = current->getNext(); 
    }
	return NULL;
}

template <typename K, typename V, typename K_comparator>
CellItem<K, V>* TableCell<K, V, K_comparator>::remove(K& key){
    CellItem<K, V> *current, *prev;
    if (listRoot == NULL){
        return NULL;
    }
	prev = listRoot;
    K_comparator Kcomp;
    if (Kcomp(*listRoot->getKey(), key)){
		current = listRoot->getNext();
		prev->setNext(NULL);
		listRoot = current;
        size--;
		return prev;
	}
	current = listRoot->getNext();
	while (current){
        if (Kcomp(*current->getKey(), key)){
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

template <typename K, typename V, typename K_comparator>
int TableCell<K, V, K_comparator>::getSize() const{
	return size;
}

template <typename K, typename V, typename K_comparator>
QString TableCell<K, V, K_comparator>::toString(){
	stringstream ss;
	ss << "TableCell\n{\n  hash: " << hash << "\n  size: " << size << "\n}\n";
    return QString::fromStdString(ss.str());
}

template <typename K, typename V, typename K_comparator>
vector<K* > TableCell<K, V, K_comparator>::keys(){
    vector<K* > keys;
    CellItem<K, V> *current = listRoot;
	for (int i = 0; i < size; i++){
        if (!current) { continue;}
        keys.push_back( current->getKey());
		current = current->getNext();
    }
	return keys;
}

template <typename K, typename V, typename K_comparator>
vector<pair<K*, V* > > TableCell<K, V, K_comparator>::entries(){
    vector<pair<K*, V* > > entries(size);
    CellItem<K, V> *current = listRoot;
    for (int i = 0; i < size; i++){
        if (!current) break;
        entries[i] = pair<K*, V* >(current->getKey(), current->getValue());
        current = current->getNext();
    }
    return entries;
}

template <typename K, typename V, typename K_comparator>
void TableCell<K, V, K_comparator>::clear(){
    if (listRoot != NULL){
        listRoot->clear();
        delete listRoot;
        listRoot = NULL;
    }
}

}

#endif
