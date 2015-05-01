#ifndef Serializible_h
#define Serializible_h

#include <string>

using namespace std;

#pragma once
class Serializible{
public:
        virtual string serialize()= 0;
        virtual void parse(string json)= 0;
};

#endif

