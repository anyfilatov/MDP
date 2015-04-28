#ifndef HashTable_h
#define HashTable_h

#include <string>
#include <vector>
#include <conio.h>
#include <QTextStream>
#include "TableCell.h"
#include "AbstractTableKey.h"
#include "CellItem.h"
#include <sstream>
#include "Serializible.h"
#include "JsonMethods.h"

using namespace std;

template <typename K, typename V>
class HashTable:public Serializible{

	private:

		int tableSize;
		int cellsNum;
		double coef;
        vector<TableCell<V>* > cells;
		void createCellsArray();
		int indexFor(int hash);
		bool overSize();
		void resize(int newCapacity);
        void transfer(vector<TableCell<V>* >* oldCells);
        void clean(vector<TableCell<V>* >* oldCells);

	public:
        HashTable();
		HashTable(int num);
		HashTable(int num, double coef);
        ~HashTable();
        void put(K* key, V* value);
        V* get(K* key);
        void update(K* key, V* value);
        void remove(K* key);
        vector<K*> keys();
        vector<pair<K*, V> > entries();
		bool isEmpty();
		int size();
		void setResizeCoef(int coef);
		int getResizeCoef();
        void clear();
        string serialize();
        void parse(string json);
};

template <typename K, typename V>
HashTable<K, V>::HashTable(){
    cellsNum = 10;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();

}

template <typename K, typename V>
HashTable<K, V>::HashTable(int num){
	cellsNum = (num > 1) ? num : 1;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();
}

template <typename K, typename V>
HashTable<K, V>::HashTable(int num, double coef){
	cellsNum = (num > 1) ? num : 1;
	tableSize = 0;
	this->coef = coef;
	createCellsArray();
}

template <typename K, typename V>
HashTable<K, V>::~HashTable(){
    clean(&cells);
}

template <typename K, typename V>
void HashTable<K, V>::clear(){
    clean(&cells);
    cellsNum = 10;
    tableSize = 0;
    createCellsArray();
}

template <typename K, typename V>
void HashTable<K, V>::createCellsArray(){
    cells = *(new vector<TableCell<V>* >(cellsNum));
	for (int n = 0; n < cells.size(); n++){
        cells[n] = new TableCell<V>(n);
	}
}

template <typename K, typename V>
void HashTable<K, V>::put(K* key, V* value){
	/*if (key == NULL){
		throws new NullPointerException("Key can not be null");
	}*/
    QTextStream cout(stdout);
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
	int index = indexFor(hash);
    cout << "HashTable_PUT\n  hash: " << hash << "\n  index: " << index << "\n  value: " << value << "\n  cellsSize: " << cells.size() << endl;

    if (cells[index]->add(new CellItem<V>((AbstractTableKey*) key->clone(), value))){
		tableSize++;
		cout << "  TableSize: " << tableSize << endl;
		if (overSize()){
			resize(cellsNum * 2);
		}
	}else{
		cout << "  KEY ALREADY EXISTS" << endl;
	}
}

template <typename K, typename V>
int HashTable<K, V>::indexFor(int hash){
	return hash % cellsNum;
}

template <typename K, typename V>
bool HashTable<K, V>::overSize(){
    QTextStream cout(stdout);
	cout << "  coef: " << ((double)tableSize/(double)cellsNum) << endl;
	return (((double)tableSize/(double)cellsNum) >= coef);
}

template <typename K, typename V>
vector<K*> HashTable<K, V>::keys(){
    vector<K*> keys(tableSize);
	int j = 0;
	for (int i = 0; i < cellsNum; i++){
        vector<AbstractTableKey*> cellKeys = cells[i]->keys();
        for (int k = 0; k < cellKeys.size(); k++){
            keys[j] = (K*) cellKeys[k]->clone();
			j++;
        }
	} 
	return keys;
}

template <typename K, typename V>
vector<pair<K*, V> > HashTable<K, V>::entries(){
    vector<pair<K*, V> > entries(tableSize);
    int j = 0;
    for (int i = 0; i < cellsNum; i++){
        vector<pair<AbstractTableKey*, V> > cellEntries = cells[i]->entries();
        for (int k = 0; k < cellEntries.size(); k++){
            entries[j] = pair<K*, V>((K*)cellEntries[k].first->clone(), cellEntries[k].second);
            j++;
        }
        cellEntries.clear();
    }
    return entries;
}

template <typename K, typename V>
V* HashTable<K, V>::get(K* key){
    QTextStream cout(stdout);
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
	int index = indexFor(hash);
    cout << "HashTable_GET\n  hash: " << hash << "\n  value: " << cells[index]->get(key) << endl;
    return cells[index]->get(key);
}

template <typename K, typename V>
void HashTable<K, V>::update(K* key, V* value){
    QTextStream cout(stdout);
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
    cout << "HashTable_UPDATE\n  hash: " << hash << "\n";
	int index = indexFor(hash);
    cells[index]->update(key, value);
}

template <typename K, typename V>
void HashTable<K, V>::remove(K* key){
    QTextStream cout(stdout);
    AbstractTableKey* keyWithHash = key;
    int hash = keyWithHash->hash();
	int index = indexFor(hash);
    CellItem<V>* removed = cells[index]->remove(key);
	tableSize--;
    delete [] removed;
    cout << "HashTable_REMOVE\n  hash: " << hash << "\n  tableSize: " << tableSize << endl;
}

template <typename K, typename V>
void HashTable<K, V>::resize(int newCapacity){
	/*if (tableSize == MaxCapacity){
		return;
	}*/
    QTextStream cout(stdout);
    cout << "HashTable_RESIZE\n  oldCellsNUM: " << cellsNum << "\n  newCellsNUM: " << newCapacity << endl;
	cellsNum = newCapacity;
    vector<TableCell<V>* > oldcells = cells;
	createCellsArray();
    transfer(&oldcells);
	return;
}

template <typename K, typename V>
bool HashTable<K, V>::isEmpty(){
	if (tableSize == 0) return true;
	return false;
}

template <typename K, typename V>
int HashTable<K, V>::size(){
	return tableSize;
}

template <typename K, typename V>
int HashTable<K, V>::getResizeCoef(){
	return coef;
}

template <typename K, typename V>
void HashTable<K, V>::setResizeCoef(int coef){
	this->coef = coef;
}

template <typename K, typename V>
void HashTable<K, V>::transfer(vector<TableCell<V>* >* oldCells){
    QTextStream cout(stdout);
    vector<TableCell<V>* > Cells = *oldCells;
    cout << "HashTable_TRANSFER\n  oldCellsNum: " << oldCells->size() << " \n  newCellsNum: " << cells.size() << endl;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        vector<AbstractTableKey*> cellKeys = cell->keys();
        cout << "    " << i << " oldCell;  keysNum: " << cellKeys.size() << endl;
		for (int j = 0; j < cellKeys.size(); j++){
            CellItem<V>* item = cell->remove(cellKeys[j]);

            cout << "      " << j << " key(" << cellKeys[j]->hash() << ", " << item->getValue() << ") goes to ";
			int index = indexFor(item->getKey()->hash());
            cout << index << " newCell\n";
            if (cell->listRoot == NULL) cout << "      CellRoot: NULL\n";
            cells[index]->add(item);
		}
    }
    clean(oldCells);
}

template <typename K, typename V>
void HashTable<K, V>::clean(vector<TableCell<V>* >* oldCells){
    QTextStream cout(stdout);
    cout << "HashTable_CLEAN\n";
    vector<TableCell<V>* > Cells = *oldCells;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        cell->clear();
	}
    oldCells->clear();
}

template <typename K, typename V>
string HashTable<K, V>::serialize(){
    vector<pair<string, string> > pairs = *(new vector<pair<string, string> >());
    vector<K* > keys = this->keys();
    for (int i = 0; i < keys.size(); i++){
        AbstractTableKey* key = keys[i];
        Serializible* value = this->get((K*)key);
        pairs.push_back(pair<string, string>(key->serialize(),value->serialize()));
    }
    string json = JsonMethods::toJsonString(pairs);
    pairs.clear();
    return json;
}

template <typename K, typename V>
void HashTable<K, V>::parse(string json){
    this->clear();
    QTextStream cout(stdout);
    vector<pair<string, string> > pairs = JsonMethods::parseJson(json);
    cout << "str: " << QString::fromStdString(json) << endl;
    for (int i = 0; i <pairs.size(); i++){
        cout << "PAIR KEY: " << QString::fromStdString(pairs[i].first) << endl;
        cout << "PAIR VALUE: "<< QString::fromStdString(pairs[i].second) << endl;
        Serializible* key = new K();
        Serializible* value = new V();
        key->parse(pairs[i].first);
        value->parse(pairs[i].second);
        this->put((K*)key, (V*)value);
    }
    pairs.clear();
}


#endif
