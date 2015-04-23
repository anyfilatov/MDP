#ifndef IntWithHash_h
#define IntWithHash_h

#include "TableKey.h"

class IntWithHash: public TableKey{
private:
	int value;
public:
	IntWithHash();
	IntWithHash(int val);
    ~IntWithHash();
	int hash();
    Clonable* clone();
	int getValue();
	void setValue(int val);
};

IntWithHash::IntWithHash(){
	value = 0;
}

IntWithHash::IntWithHash(int val){
	value = val;
}

IntWithHash::~IntWithHash(){
}

int IntWithHash::getValue(){
	return value;
}

void IntWithHash::setValue(int val){
	value = val;
}

int IntWithHash::hash(){
	return value;
}\

Clonable* IntWithHash::clone(){
    return new IntWithHash(value);
}

#endif
