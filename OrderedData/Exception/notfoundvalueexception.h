#ifndef NOTFOUNDVALUEEXCEPTION_H
#define NOTFOUNDVALUEEXCEPTION_H
#include "clientexeption.h"

class NotFoundValueException: public ClientExeption
{
public:
    NotFoundValueException();
    ~NotFoundValueException();
};

#endif // NOTFOUNDVALUEEXCEPTION_H
