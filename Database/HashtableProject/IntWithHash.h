#include "d:\github\mdp\mdp\database\hashtableproject\hashable.h"

class IntWithHash:public Hashable{
private:
	int value;
public:
	IntWithHash();
	IntWithHash(int val);
	int hash();
	int getValue();
	void setValue(int val);
};

IntWithHash::IntWithHash(){
	value = 0;
}

IntWithHash::IntWithHash(int val){
	value = val;
}

int IntWithHash::getValue(){
	return value;
}

void IntWithHash::setValue(int val){
	value = val;
}

int IntWithHash::hash(){
	return value;
}