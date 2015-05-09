#ifndef CLIENTEXEPTION_H
#define CLIENTEXEPTION_H
#include "abstractexception.h"

class ClientExeption : public AbstractException
{
public:
    ClientExeption();
    ~ClientExeption();
protected:
    void init(ExceptionCode exp, bool children = true);
};

#endif // CLIENTEXEPTION_H
