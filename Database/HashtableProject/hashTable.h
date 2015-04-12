#include <string>
#include "stdafx.h"
#include <vector>

using namespace std;

template < typename V>
class HashTable{

	private:

		int tableSize;
		int cellsNum;
		vector<TableCell<V>> cells;
		void createCellsArray();

	public:
		HashTable();
		HashTable(int num);
		void put(Hashable*  key, V value);
		V get(Hashable*  key);
		void update(Hashable*  key, V value);
		V remove(Hashable*  key);
		vector<Hashable*>	keys();
		bool isEmpty();
		int size();
		void resize(int newCapacity);
		void transfer(vector<TableCell<V>> oldcells);
};

template <typename V>
HashTable<V>::HashTable(){
	cellsNum = 10;
	createCellsArray();

}

template < typename V>
HashTable<V>::HashTable(int num){
	cellsNum = (num > 1) ? num : 1;
	createCellsArray();
}

template < typename V>
void HashTable<V>::createCellsArray(){
	cells = *(new vector<TableCell<V>>[cellsNum]);
	for (int n = 0; n < cells.size(); n++){
		cells[n] = *(new TableCell<V>(n));
	}
}
template <typename V>
void HashTable<V>::put(Hashable*  key, V value){
	if (key==null){
		throws new NullPointerException("Key can not be null");
	}
	int hash = K.hash();
	int index = indexFor(hash, tableSize);
	bool added = cells[index].addItem(new CellItem(key, value));
	if (added){
		tableSize++;
	}
	if (added && overSize()){
		resize(tableSize*2);
	}
}

template <typename V>
vector<Hashable*> HashTable<V>::keys(){
	vector<Hashable* > keys  = new vector<Hashable* >[tableSize];
	int j = 0;
	for (int i=0; i<cellsNum; i++){
		vector<Hashable* > cellKeys= cells[i].getKeys();
		int cellKeysSize = cells[i].getKeysSize();
		for (int k = 0; k<cellKeysSize; k++){
			keys[j] = cellKeys[k];
			j++;
		}
	} 
	return keys;
}

template < typename V>
V HashTable<V>::get(Hashable*  key){
	int hash = key.hash();
	int index = indexFor(hash);
	return cells[index].get(key);
}

template < typename V>
void HashTable<V>::update(Hashable*  key, V value){
	int hash = key.hash();
	int index = indexFor(hash);
	return cells[index].update(key, value);
}

template < typename V>
void HashTable<V>::resize (int newCapacity){
	if (tableSize == MaxCapacity){
		return;
	}
	сellsNum = newCapacity;
	TableCell[] oldcells = cells;
	createCellsArray();
	transfer(oldcells);
	//надо удалять только тэблселы, а селлитемы удалять не надо, в них надо только заменить ссылки на следующий
//	clean(oldcells);
	return;
}

template < typename V>
V HashTable<V>::remove(Hashable*  key){
	int hash = key.hash();
	int index = indexFor(hash);
	return cells[index].remove(key);
}


template < typename V>
bool HashTable<V>::isEmpty(){
	if (tableSize==0) return true;
	return false;
}

template < typename V>
int HashTable<V>::size(){
	return tableSize;
}