#ifndef StringWithHash_h
#define StringWithHash_h

#include "AbstractTableKey.h"
#include <sstream>
#include <string>
#include <vector>
#include "JsonMethods.h"

using namespace std;

class StringWithHash:public AbstractTableKey{
private:
    string value;

public:
    StringWithHash();
    StringWithHash(string val);
    ~StringWithHash();
    string getValue();
    void setValue(string val);
    int hash();
    Clonable* clone();
    bool equals(AbstractTableKey* other);
    string serialize();
    void parse(string json);
};

StringWithHash::StringWithHash(){
    value = "";
}

StringWithHash::StringWithHash(string val){
    value = val;
}

StringWithHash::~StringWithHash(){
}

string StringWithHash::getValue(){
    return value;
}

void StringWithHash::setValue(string val){
    value = val;
}

int StringWithHash::hash(){
    return value.length();
}\

Clonable* StringWithHash::clone(){
    return new StringWithHash(value);
}

bool StringWithHash::equals(AbstractTableKey* other){
    StringWithHash* otherString = (StringWithHash*) other;
    return (value == otherString->getValue());
}

string StringWithHash::serialize(){
    return value;
}

void StringWithHash::parse(string json){
    value = json;
}

#endif
