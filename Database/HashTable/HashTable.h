#ifndef HashTable_h
#define HashTable_h

#include <QStringList>
#include <QString>
#include <vector>
#include <QTextStream>
#include "HashTable/TableCell.h"
#include "HashTable/AbstractTableKey.h"
#include "HashTable/CellItem.h"
#include <sstream>
#include "Serializible.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QDebug>
#include <ostream>
#include "Exceptions/NotUniqueKeyException.h"
#include "Exceptions/NullPointerException.h"

using namespace std;

namespace database{

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
class HashTable;

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
ostream& operator << (ostream& stream, const HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>& table);

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
class HashTable{

	private:
		int tableSize;
        int cellsCount;
		double coef;
        vector<TableCell<K, V, K_comparator>* > cells;
		void createCellsArray();
        int indexFor(int hash) const;
        bool overSize() const;
		void resize(int newCapacity);
        void transfer(vector<TableCell<K, V, K_comparator>* >* oldCells);
        void clean(vector<TableCell<K, V, K_comparator>* >* oldCells);
    public:
        class Iterator;
        HashTable();
		HashTable(int num);
		HashTable(int num, double coef);
        ~HashTable();
        Iterator iterator(){
            HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::Iterator iter(this);
            return iter;
        }
        void put(K& key, V& value);
        V& get(K key);
        void remove(K& key);
        bool contains(K key) const;
        vector<K> keys();
        vector<pair<K, V> > entries();
        bool isEmpty() const;
        int size() const;
        void setResizeCoef(double coef);
        double getResizeCoef() const;
        void clear();
        bool operator ==(HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>& another);
        friend ostream& operator << <>(ostream& stream, const HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>& table);
};

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
ostream& operator << (ostream& stream, const HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>& table)
{
    stream << "HashTable size: " << table.size();
    return stream;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
class HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::Iterator{
private:
    HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>* table;
    int oldTableSize;
    int oldCellsCount;
    int cellNum;
    bool permanent;
    int num;
    CellItem<K, V>* currentItem;
    bool checkPepmanent(){
        if ((!permanent) || (!table) || (oldTableSize != table->tableSize) || (oldCellsCount != table->cellsCount) || (!currentItem)){
            permanent = false;
            return false;
        } else return true;
    }
public:
    Iterator(HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>* outerTable){
        table = outerTable;
        oldTableSize = outerTable->tableSize;
        oldCellsCount = outerTable->cellsCount;
        num = 0;
        if (oldTableSize > 0){
            cellNum = 0;
            while(!outerTable->cells[cellNum]->listRoot){
                cellNum++;
            }
            currentItem = outerTable->cells[cellNum]->listRoot;
        }else{
            currentItem = NULL;
        }
        permanent = true;
    }

    V& next(){
        V v;
        if (num >= oldTableSize - 1) return v;
        num++;
        if (checkPepmanent()){
            if (currentItem->getNext()){
                currentItem = currentItem->getNext();
                return *(currentItem->getValue());
            }else if (cellNum >= oldCellsCount){
                return v;
            }else{
                cellNum++;
                while(!table->cells[cellNum]->listRoot){
                    cellNum++;
                }
                currentItem = table->cells[cellNum]->listRoot;
                return *(currentItem->getValue());
            }
        }else return v;
    }

    bool hasNext(){
        if (checkPepmanent()){
            if (((!currentItem->getNext()) && (cellNum >= oldCellsCount)) || (num >= oldTableSize - 1))
                return false;
            else
                return true;
        }else return false;
    }

    bool isFirst(){
        return (num == 0);
    }

    K getKey(){
        if (currentItem){
            return *(currentItem->getKey());
        }else{
            K k;
            return k;
        }
    }

    V& getValue(){
        if (currentItem){
            return *(currentItem->getValue());
        }else{
            V v;
            return v;
        }
    }
};

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::HashTable(){
    cellsCount = 30;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();

}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::HashTable(int num){
    cellsCount = (num > 1) ? num : 1;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::HashTable(int num, double coef){
    cellsCount = (num > 1) ? num : 1;
	tableSize = 0;
	this->coef = coef;
	createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::~HashTable(){
    clean(&cells);
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
bool HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::operator==(HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>& another){
        if (tableSize != another.size()) return false;
        HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::Iterator outerIt = another.iterator();
        V_comparator Vcomp;
        while(true){
            if (!this->contains(outerIt.getKey()) || !Vcomp(outerIt.getValue(), this->get(outerIt.getKey()))) return false;
            if(outerIt.hasNext())
                outerIt.next();
            else break;
        }

       return true;

    }

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::clear(){
    clean(&cells);
    cellsCount = 10;
    tableSize = 0;
    createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::createCellsArray(){
    cells.clear();
    cells.resize(cellsCount);
	for (int n = 0; n < cells.size(); n++){
        cells[n] = new TableCell<K, V, K_comparator>(n);
	}
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::put(K& key, V& value){
    hashFunc hash;
    K_cloneFunc K_clone;
    V_cloneFunc V_clone;
    int index = indexFor(hash(key));
    if (cells[index]->add(new CellItem<K, V>(K_clone(key), V_clone(value)))){
        tableSize++;
        if (overSize()){
            resize(cellsCount * 2);
        }
    }
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
int HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::indexFor(int hash) const{
    return hash % cellsCount;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
bool HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::overSize() const{
    return (((double)tableSize/(double)cellsCount) >= coef);
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
vector<K> HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::keys(){
    vector<K> Keys;
	int j = 0;
    for (int i = 0; i < cellsCount; i++){
        vector<K* > cellKeys = cells[i]->keys();
        for (int k = 0; k < cellKeys.size(); k++){
            K* key = cellKeys[k];
            Keys.push_back(*key);
			j++;
        }
    }
    return Keys;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
vector<pair<K, V> > HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::entries(){
    vector<pair<K, V> > entries;
    int j = 0;
    for (int i = 0; i < cellsCount; i++){
        vector<pair<K*, V* > > cellEntries = cells[i]->entries();
        for (int k = 0; k < cellEntries.size(); k++){
            K* key = cellEntries[k].first;
            V* value = cellEntries[k].second;
            if (key && value)
                entries.push_back(pair<K, V>(*key, *value));
            j++;
        }
    }
    return entries;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
V& HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::get(K key){
    hashFunc hash;
    int index = indexFor(hash(key));
    V* value = cells[index]->get(key);
    if (value) return *value;
    V v;
    return v;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::remove(K& key){
    hashFunc hash;
    int index = indexFor(hash(key));
    CellItem<K, V>* removed = cells[index]->remove(key);
    if (removed != NULL){
        tableSize--;
        removed->clear();
        delete removed;
    }
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
bool HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::contains(K key) const{
    hashFunc hash;
    int index = indexFor(hash(key));
    V* value = cells[index]->get(key);
    if (value) return true;
    return false;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::resize(int newCapacity){
    cellsCount = newCapacity;
    vector<TableCell<K, V, K_comparator>* > oldcells(cells.size());
    for (int i = 0; i < cells.size(); i++){
        oldcells[i] = cells[i];
        cells[i] = NULL;
    }
	createCellsArray();
    transfer(&oldcells);
	return;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
bool HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::isEmpty() const{
	if (tableSize == 0) return true;
	return false;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
int HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::size() const{
	return tableSize;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
double HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::getResizeCoef() const{
	return coef;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::setResizeCoef(double coef){
	this->coef = coef;
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::transfer(vector<TableCell<K, V, K_comparator>* >* oldCells){
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<K, V, K_comparator>* cell = oldCells->at(i);
        vector<K* > cellKeys = cell->keys();
		for (int j = 0; j < cellKeys.size(); j++){
            CellItem<K, V>* item = cell->remove(*cellKeys[j]);
            int index = indexFor(item->getKey()->hash());
            cells[index]->add(item);
		}
    }
    clean(oldCells);
}

template <typename K, typename V, typename K_comparator, typename V_comparator, typename hashFunc, typename K_cloneFunc, typename V_cloneFunc>
void HashTable<K, V, K_comparator, V_comparator, hashFunc, K_cloneFunc, V_cloneFunc>::clean(vector<TableCell<K, V, K_comparator>* >* oldCells){
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<K, V, K_comparator>* cell = oldCells->at(i);
        cell->clear();
        delete cell;
	}
    oldCells->clear();
}

}

#endif
