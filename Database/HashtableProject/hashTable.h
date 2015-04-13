#include <string>
#include "stdafx.h"
#include <vector>
#include "TableCell.h"
#include "Hashable.h"
#include <conio.h>

using namespace std;

template <typename V>
class HashTable{

	private:

		int tableSize;
		int cellsNum;
		double coef;
		vector<TableCell<V>> cells;
		void createCellsArray();
		int indexFor(int hash);
		bool overSize();
		void resize(int newCapacity);
		void transfer(vector<TableCell<V>> oldCells);

	public:
		HashTable();
		HashTable(int num);
		HashTable(int num, double coef);
		void put(Hashable*  key, V value);
		V get(Hashable* key);
		void update(Hashable* key, V value);
		void remove(Hashable* key);
		vector<Hashable*> keys();
		bool isEmpty();
		int size();
		void setResizeCoef(int coef);
		int getResizeCoef();
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
void HashTable<V>::createCellsArray(){
	cells = *(new vector<TableCell<V>>(cellsNum));
	for (int n = 0; n < cells.size(); n++){
		cells[n] = *(new TableCell<V>(n));
	}
}
template <typename V>
void HashTable<V>::put(Hashable* key, V value){
	/*if (key == NULL){
		throws new NullPointerException("Key can not be null");
	}*/
	int hash = key->hash();
	int index = indexFor(hash);
	cout << "PUT\n  hash: " << hash << "\n  index: " << index << "\n  value: " << value << "\n  cellsSize: " << cells.size() << endl;

	if (cells[index].add(new CellItem<int>(key, value))){
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
	cout << "  coef: " << ((double)tableSize/(double)cellsNum) << endl;
	return (((double)tableSize/(double)cellsNum) >= coef);
}

template <typename V>
vector<Hashable*> HashTable<V>::keys(){
	vector<Hashable*> keys(tableSize);
	int j = 0;
	for (int i = 0; i < cellsNum; i++){
		vector<Hashable*> cellKeys = cells[i].keys();
		for (int k = 0; k < cellKeys.size(); k++){
			keys[j] = cellKeys[k];
			j++;
		}
	} 
	return keys;
}

template <typename V>
V HashTable<V>::get(Hashable* key){
	int hash = key->hash();
	int index = indexFor(hash);
	cout << "GET\n  hash: " << hash << "\n  value: " << cells[index].get(key) << endl;
	return cells[index].get(key);
}

template <typename V>
void HashTable<V>::update(Hashable* key, V value){
	int hash = key->hash();
	cout << "UPDATE\n  hash: " << hash << "\n  value: " << value << endl;
	int index = indexFor(hash);
	cells[index].update(key, value);
}

template <typename V>
void HashTable<V>::remove(Hashable* key){
	int hash = key->hash();
	int index = indexFor(hash);
	CellItem<V>* removed = cells[index].remove(key);
	tableSize--;
	delete removed;
	cout << "REMOVE\n  hash: " << hash << "\n  tableSize: " << tableSize << endl;
}

template < typename V>
void HashTable<V>::resize(int newCapacity){
	/*if (tableSize == MaxCapacity){
		return;
	}*/
	cout << "RESIZE\n  oldCellsNUM: " << cellsNum << "\n  newCellsNUM: " << newCapacity << endl;
	cellsNum = newCapacity;
	vector<TableCell<V>> oldcells = cells;
	createCellsArray();
	transfer(oldcells);
	//надо удалять только тэблселы, а селлитемы удалять не надо, в них надо только заменить ссылки на следующий
//	clean(oldcells);
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
void HashTable<V>::transfer(vector<TableCell<V>> oldCells){
	cout << "TRANSFER\n oldCellsNum: " << oldCells.size() << " \n newCellsNum: " << cells.size() << endl;
	for (int i = 0; i < oldCells.size(); i++){
		TableCell<V> cell = oldCells[i];
		vector<Hashable*> cellKeys = cell.keys();
		cout << "  " << i << " oldCell;  keysNum: " << cellKeys.size() << endl;
		for (int j = 0; j < cellKeys.size(); j++){
			cout << "    " << j << " key(" << cellKeys[j]->hash() << ", " << cell.get(cellKeys[j]) << ") goes to ";
			CellItem<V>* item = cell.remove(cellKeys[j]);
			int index = indexFor(item->getKey()->hash());
			cout << index << " newCell\n";  
			cells[index].add(item);
		}
	}
}