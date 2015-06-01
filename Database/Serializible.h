#ifndef Serializible_h
#define Serializible_h

#include <QString>

using namespace std;
namespace database {

#pragma once
class Serializible{
public:
        virtual QString serialize()= 0;
        virtual void parse(QString json)= 0;
};

}
#endif

