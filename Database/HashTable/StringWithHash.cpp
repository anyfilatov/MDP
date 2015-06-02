#ifndef StringWithHash_cpp
#define StringWithHash_cpp

#include "AbstractTableKey.h"
#include <sstream>
#include <QString>
#include <vector>
#include "StringWithHash.h"

using namespace std;

namespace database{

StringWithHash::StringWithHash(){
    value = "";
}

StringWithHash::StringWithHash(QString val){
    value = val;
}

StringWithHash::~StringWithHash(){
}

const QString& StringWithHash::getValue() const{
    return value;
}

void StringWithHash::setValue(QString val){
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

QString StringWithHash::serialize(){
    return value;
}

void StringWithHash::parse(QString json){
    value = json;
}

}

#endif

