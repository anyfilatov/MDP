#ifndef CONNECTIONISNOTCREATEDEXCEPTION_H
#define CONNECTIONISNOTCREATEDEXCEPTION_H
#include "clientexeption.h"

class ConnectionIsNotCreatedException: public ClientExeption
{
public:
    ConnectionIsNotCreatedException();
    ~ConnectionIsNotCreatedException();
};

#endif // CONNECTIONISNOTCREATEDEXCEPTION_H
