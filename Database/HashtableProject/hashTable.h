#ifndef HashTable_h
#define HashTable_h

#include <string>
#include <vector>
#include <conio.h>
#include <QTextStream>
#include "TableCell.h"
#include "TableKey.h"
#include "CellItem.h"

using namespace std;

template <typename V>
class HashTable{

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
        void put(TableKey* key, V value);
        V get(TableKey* key);
        void update(TableKey* key, V value);
        void remove(TableKey* key);
        vector<TableKey*> keys();
        vector<pair<TableKey*, V> > entries();
		bool isEmpty();
		int size();
		void setResizeCoef(int coef);
		int getResizeCoef();
        void clear();
};

template <typename V>
HashTable<V>::HashTable(){
	cellsNum = 10;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();

}

template < typename V>
HashTable<V>::HashTable(int num){
	cellsNum = (num > 1) ? num : 1;
	tableSize = 0;
	coef = 0.6;
	createCellsArray();
}

template < typename V>
HashTable<V>::HashTable(int num, double coef){
	cellsNum = (num > 1) ? num : 1;
	tableSize = 0;
	this->coef = coef;
	createCellsArray();
}

template < typename V>
HashTable<V>::~HashTable(){
    clean(&cells);
}

template < typename V>
void HashTable<V>::clear(){
    clean(&cells);
    cellsNum = 10;
    tableSize = 0;
    createCellsArray();
}

template <typename V>
void HashTable<V>::createCellsArray(){
    cells = *(new vector<TableCell<V>* >(cellsNum));
	for (int n = 0; n < cells.size(); n++){
        cells[n] = new TableCell<V>(n);
	}
}

template <typename V>
void HashTable<V>::put(TableKey* key, V value){
	/*if (key == NULL){
		throws new NullPointerException("Key can not be null");
	}*/
    QTextStream cout(stdout);
    int hash = key->hash();
	int index = indexFor(hash);
    cout << "PUT\n  hash: " << hash << "\n  index: " << index << "\n  value: " << value << "\n  cellsSize: " << cells.size() << endl;

    if (cells[index]->add(new CellItem<int>((TableKey*) key->clone(), value))){
		tableSize++;
		cout << "  TableSize: " << tableSize << endl;
		if (overSize()){
			resize(cellsNum * 2);
		}
	}else{
		cout << "  KEY ALREADY EXISTS" << endl;
	}
}

template <typename V>
int HashTable<V>::indexFor(int hash){
	return hash % cellsNum;
}

template <typename V>
bool HashTable<V>::overSize(){
    QTextStream cout(stdout);
	cout << "  coef: " << ((double)tableSize/(double)cellsNum) << endl;
	return (((double)tableSize/(double)cellsNum) >= coef);
}

template <typename V>
vector<TableKey*> HashTable<V>::keys(){
    vector<TableKey*> keys(tableSize);
	int j = 0;
	for (int i = 0; i < cellsNum; i++){
        vector<TableKey*> cellKeys = cells[i]->keys();
        for (int k = 0; k < cellKeys.size(); k++){
            keys[j] = (TableKey*) cellKeys[k]->clone();
			j++;
		}
	} 
	return keys;
}

template <typename V>
vector<pair<TableKey*, V> > HashTable<V>::entries(){
    vector<pair<TableKey*, V> > entries(tableSize);
    int j = 0;
    for (int i = 0; i < cellsNum; i++){
        vector<pair<TableKey*, V> > cellEntries = cells[i]->entries();
        for (int k = 0; k < cellEntries.size(); k++){
            entries[j] = cellEntries[k];
            j++;
        }
    }
    return entries;
}

template <typename V>
V HashTable<V>::get(TableKey* key){
    QTextStream cout(stdout);
    int hash = key->hash();
	int index = indexFor(hash);
    cout << "GET\n  hash: " << hash << "\n  value: " << cells[index]->get(key) << endl;
    return cells[index]->get(key);
}

template <typename V>
void HashTable<V>::update(TableKey* key, V value){
    QTextStream cout(stdout);
    int hash = key->hash();
	cout << "UPDATE\n  hash: " << hash << "\n  value: " << value << endl;
	int index = indexFor(hash);
    cells[index]->update(key, value);
}

template <typename V>
void HashTable<V>::remove(TableKey* key){
    QTextStream cout(stdout);
    int hash = key->hash();
	int index = indexFor(hash);
    CellItem<V>* removed = cells[index]->remove(key);
	tableSize--;
    delete [] removed;
	cout << "REMOVE\n  hash: " << hash << "\n  tableSize: " << tableSize << endl;
}

template < typename V>
void HashTable<V>::resize(int newCapacity){
	/*if (tableSize == MaxCapacity){
		return;
	}*/
    QTextStream cout(stdout);
	cout << "RESIZE\n  oldCellsNUM: " << cellsNum << "\n  newCellsNUM: " << newCapacity << endl;
	cellsNum = newCapacity;
    vector<TableCell<V>* > oldcells = cells;
	createCellsArray();
    transfer(&oldcells);
	return;
}

template <typename V>
bool HashTable<V>::isEmpty(){
	if (tableSize == 0) return true;
	return false;
}

template <typename V>
int HashTable<V>::size(){
	return tableSize;
}

template <typename V>
int HashTable<V>::getResizeCoef(){
	return coef;
}

template <typename V>
void HashTable<V>::setResizeCoef(int coef){
	this->coef = coef;
}

template < typename V>
void HashTable<V>::transfer(vector<TableCell<V>* >* oldCells){
    QTextStream cout(stdout);
    vector<TableCell<V>* > Cells = *oldCells;
    cout << "TRANSFER\n oldCellsNum: " << oldCells->size() << " \n newCellsNum: " << cells.size() << endl;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        vector<TableKey*> cellKeys = cell->keys();
		cout << "  " << i << " oldCell;  keysNum: " << cellKeys.size() << endl;
		for (int j = 0; j < cellKeys.size(); j++){
            CellItem<V>* item = cell->remove(cellKeys[j]);

            cout << "    " << j << " key(" << cellKeys[j]->hash() << ", " << cell->get(cellKeys[j]) << ") goes to ";
			int index = indexFor(item->getKey()->hash());
            cout << index << " newCell\n";
            if (cell->listRoot == NULL) cout << "  cellRoot: NULL\n";
            cells[index]->add(item);
		}
    }
    clean(oldCells);
}

template < typename V>
void HashTable<V>::clean(vector<TableCell<V>* >* oldCells){
    QTextStream cout(stdout);
	cout << "CLEAN\n";
    vector<TableCell<V>* > Cells = *oldCells;
    for (int i = 0; i < oldCells->size(); i++){
        TableCell<V>* cell = Cells[i];
        cell->clear();
	}
    oldCells->clear();
}

#endif
