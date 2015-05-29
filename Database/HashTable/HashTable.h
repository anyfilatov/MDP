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

using namespace std;

namespace database{

template <typename K, typename V, typename K_comparator, typename V_comparator>
class HashTable;

template <typename K, typename V, typename K_comparator, typename V_comparator>
ostream& operator << (ostream& stream, const HashTable<K, V, K_comparator, V_comparator>& table);

template <typename K, typename V, typename K_comparator, typename V_comparator>
class HashTable:public Serializible{

	private:
		int tableSize;
        int cellsCount;
		double coef;
        vector<TableCell<V>* > cells;
		void createCellsArray();
		int indexFor(int hash);
		bool overSize();
		void resize(int newCapacity);
        void transfer(vector<TableCell<V>* >* oldCells);
        void clean(vector<TableCell<V>* >* oldCells);
    public:
        class Iterator;
        HashTable();
		HashTable(int num);
		HashTable(int num, double coef);
        ~HashTable();
        Iterator iterator(){
            HashTable<K, V, K_comparator, V_comparator>::Iterator iter(this);
            return iter;
        }
        void put(K* key, V* value);
        V* get(K* key);
        void update(K* key, V* value);
        void remove(K* key);
        bool contains(K* key);
        vector<K> keys();
        vector<pair<K, V> > entries();
        bool isEmpty() const;
        int size() const;
        void setResizeCoef(double coef);
        double getResizeCoef() const;
        void clear();
        bool operator ==(HashTable<K, V, K_comparator, V_comparator>& another);
        QString serialize();
        void parse(QString json);
        friend ostream& operator << <>(ostream& stream, const HashTable<K, V, K_comparator, V_comparator>& table);
};

template <typename K, typename V, typename K_comparator, typename V_comparator>
ostream& operator << (ostream& stream, const HashTable<K, V, K_comparator, V_comparator>& table)
{
    stream << "HashTable size: " << table.size();
    return stream;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
class HashTable<K, V, K_comparator, V_comparator>::Iterator{
private:
    HashTable<K, V, K_comparator, V_comparator>* table;
    int oldTableSize;
    int oldCellsCount;
    int cellNum;
    bool permanent;
    int num;
    CellItem<V>* currentItem;
    bool checkPepmanent(){
        if ((!permanent) || (!table) || (oldTableSize != table->tableSize) || (oldCellsCount != table->cellsCount) || (!currentItem)){
            permanent = false;
            return false;
        } else return true;
    }
public:
    Iterator(HashTable<K, V, K_comparator, V_comparator>* outerTable){
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

    V* next(){
        if (num >= oldTableSize - 1) return NULL;
        num++;
        if (checkPepmanent()){
            if (currentItem->getNext()){
                currentItem = currentItem->getNext();
                return currentItem->getValue();
            }else if (cellNum >= oldCellsCount){
                return NULL;
            }else{
                cellNum++;
                while(!table->cells[cellNum]->listRoot){
                    cellNum++;
                }
                currentItem = table->cells[cellNum]->listRoot;
                return currentItem->getValue();
            }
        }else return NULL;
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

    K* getKey(){
        if (currentItem){
            return (K*)currentItem->getKey()->clone();
        }else{
            return NULL;
        }
    }

    V* getValue(){
        if (currentItem){
            return currentItem->getValue();
        }else{
            return NULL;
        }
    }
};

template <typename K, typename V, typename K_comparator, typename V_comparator>
HashTable<K, V, K_comparator, V_comparator>::HashTable(){
    cellsCount = 30;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();

}

template <typename K, typename V, typename K_comparator, typename V_comparator>
HashTable<K, V, K_comparator, V_comparator>::HashTable(int num){
    cellsCount = (num > 1) ? num : 1;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
HashTable<K, V, K_comparator, V_comparator>::HashTable(int num, double coef){
    cellsCount = (num > 1) ? num : 1;
	tableSize = 0;
	this->coef = coef;
	createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
HashTable<K, V, K_comparator, V_comparator>::~HashTable(){
    clean(&cells);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
bool HashTable<K, V, K_comparator, V_comparator>::operator==(HashTable<K, V, K_comparator, V_comparator>& another){
        if (tableSize != another.size() || coef != another.getResizeCoef()) return false;
        HashTable<K, V, K_comparator, V_comparator>::Iterator innerIt = this->iterator();
        HashTable<K, V, K_comparator, V_comparator>::Iterator outerIt = another.iterator();
        K_comparator Kcomp;
        V_comparator Vcomp;
        while(outerIt.hasNext() && innerIt.hasNext()){
            if(outerIt.isFirst()){
                if (!Vcomp(*(outerIt.getValue()), *(innerIt.getValue()))
                        || !Kcomp(*(outerIt.getKey()), *(innerIt.getKey()))) return false;
            }
            if (!Vcomp(*(outerIt.next()), *(innerIt.next()))
                    || !Kcomp(*(outerIt.getKey()), *(innerIt.getKey()))) return false;
        }

       return true;

    }

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::clear(){
    clean(&cells);
    cellsCount = 10;
    tableSize = 0;
    createCellsArray();
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::createCellsArray(){
    cells.clear();
    cells.resize(cellsCount);
	for (int n = 0; n < cells.size(); n++){
        cells[n] = new TableCell<V>(n);
	}
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::put(K* key, V* value){
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
    int index = indexFor(hash);
    if (cells[index]->add(new CellItem<V>((AbstractTableKey*) key->clone(), value))){
        tableSize++;
		if (overSize()){
            resize(cellsCount * 2);
		}
    }
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
int HashTable<K, V, K_comparator, V_comparator>::indexFor(int hash){
    return hash % cellsCount;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
bool HashTable<K, V, K_comparator, V_comparator>::overSize(){
    return (((double)tableSize/(double)cellsCount) >= coef);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
vector<K> HashTable<K, V, K_comparator, V_comparator>::keys(){
    vector<K> Keys;
	int j = 0;
    for (int i = 0; i < cellsCount; i++){
        vector<AbstractTableKey*> cellKeys = cells[i]->keys();
        for (int k = 0; k < cellKeys.size(); k++){
            K* key = (K*) cellKeys[k];
            Keys.push_back(*key);
			j++;
        }
    }
    return Keys;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
vector<pair<K, V> > HashTable<K, V, K_comparator, V_comparator>::entries(){
    vector<pair<K, V> > entries(tableSize);
    int j = 0;
    for (int i = 0; i < cellsCount; i++){
        vector<pair<AbstractTableKey*, V> > cellEntries = cells[i]->entries();
        for (int k = 0; k < cellEntries.size(); k++){
            K* key = (K*) cellEntries[k].first;
            entries[j] = pair<K, V>(*key, cellEntries[k].second);
            j++;
        }
        cellEntries.clear();
    }
    return entries;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
V* HashTable<K, V, K_comparator, V_comparator>::get(K* key){
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
    int index = indexFor(hash);
    return cells[index]->get(key);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::update(K* key, V* value){
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
	int index = indexFor(hash);
    cells[index]->update(key, value);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::remove(K* key){
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
	int index = indexFor(hash);
    CellItem<V>* removed = cells[index]->remove(key);
    if (removed != NULL){
        tableSize--;
        delete removed;
    }
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
bool HashTable<K, V, K_comparator, V_comparator>::contains(K* key){
    return (this->get(key) != NULL);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::resize(int newCapacity){
    cellsCount = newCapacity;
    vector<TableCell<V>* > oldcells(cells.size());
    for (int i = 0; i < cells.size(); i++){
        oldcells[i] = cells[i];
        cells[i] = NULL;
    }
	createCellsArray();
    transfer(&oldcells);
	return;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
bool HashTable<K, V, K_comparator, V_comparator>::isEmpty() const{
	if (tableSize == 0) return true;
	return false;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
int HashTable<K, V, K_comparator, V_comparator>::size() const{
	return tableSize;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
double HashTable<K, V, K_comparator, V_comparator>::getResizeCoef() const{
	return coef;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::setResizeCoef(double coef){
	this->coef = coef;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::transfer(vector<TableCell<V>* >* oldCells){
    vector<TableCell<V>* > Cells = *oldCells;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        vector<AbstractTableKey*> cellKeys = cell->keys();
		for (int j = 0; j < cellKeys.size(); j++){
            CellItem<V>* item = cell->remove(cellKeys[j]);
            int index = indexFor(item->getKey()->hash());
            cells[index]->add(item);
		}
    }
    clean(oldCells);
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::clean(vector<TableCell<V>* >* oldCells){
    vector<TableCell<V>* > Cells = *oldCells;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        cell->clear();
	}
    oldCells->clear();
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
QString HashTable<K, V, K_comparator, V_comparator>::serialize(){
    QJsonObject jsonObj;
    vector<K> keys = this->keys();
    for (int i = 0; i < keys.size(); i++){
        AbstractTableKey* key = &keys[i];
        Serializible* value = this->get((K*) key);
        jsonObj.insert(key->serialize(),value->serialize());
    }
    QJsonDocument jdoc(jsonObj);
    QString str(jdoc.toJson());
    return str;
}

template <typename K, typename V, typename K_comparator, typename V_comparator>
void HashTable<K, V, K_comparator, V_comparator>::parse(QString json){
    this->clear();
    QJsonDocument jdoc;
    jdoc = jdoc.fromJson(json.toUtf8());
    QJsonObject jsonObj;
    jsonObj = jdoc.object();

    QStringList keys = jsonObj.keys();
    cellsCount = keys.size() * 2;
    tableSize = keys.size();
    createCellsArray();

    for (int i = 0; i <keys.size(); i++){
        QJsonValue val = jsonObj.take(keys[i]);
        Serializible* key = new K();
        Serializible* value = new V();
        key->parse(keys[i]);
        value->parse(val.toString());
        this->put((K*)key, (V*)value);
        delete key;
    }
}

}

#endif
