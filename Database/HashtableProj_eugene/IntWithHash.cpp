#ifndef IntWithHash_cpp
#define IntWithHash_cpp

#include "AbstractTableKey.h"
//#include "JsonMethods.h"
#include <iostream>
#include <sstream>
#include <QString>
#include "IntWithHash.h"

using namespace std;

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

QString IntWithHash::serialize(){
    return QString::number(value);
}

void IntWithHash::parse(QString json){
    value = json.toInt();
}

#endif
