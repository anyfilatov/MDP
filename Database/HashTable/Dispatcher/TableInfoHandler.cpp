#ifndef TABLEINFOHANDLER_CPP
#define TABLEINFOHANDLER_CPP

#include "Serializible.h"
#include "TableInfoHandler.h"
#include <vector>

using namespace std;

TableInfoHandler::TableInfoHandler(int count, vector<pair<int, int> > firstIndexes){
    stringsCount = count;
    this->firstIndexes = firstIndexes;
}

TableInfoHandler::~TableInfoHandler(){
    firstIndexes.clear();
}

void TableInfoHandler::putStrings(int num, int count){

}

#endif // TABLEINFOHANDLER
