#ifndef IntWithHash_h
#define IntWithHash_h

#include "AbstractTableKey.h"
#include "JsonMethods.h"
#include "StringWithHash.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class IntWithHash: public AbstractTableKey{
private:
	int value;
public:
	IntWithHash();
	IntWithHash(int val);
    ~IntWithHash();
    int getValue();
    void setValue(int val);
	int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    string serialize();
    void parse(string json);
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
}

Clonable* IntWithHash::clone(){
    return new IntWithHash(value);
}

bool IntWithHash::equals(AbstractTableKey* other){
    IntWithHash* otherInt = (IntWithHash*) other;
    return (value == otherInt->getValue());
}

string IntWithHash::serialize(){
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void IntWithHash::parse(string json){
    vector<pair<string, string> > pairs = JsonMethods::parseJson(json);
    int val = 0;
    for (int i = json.size() - 1, k = 1; i >= 0; i--, k *= 10) {
        val += (json[i] - 48) * k;
    }
    value = val;\
}

#endif
