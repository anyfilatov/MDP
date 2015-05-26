#ifndef SERVERUNAVAILABLEEXCEPTION_H
#define SERVERUNAVAILABLEEXCEPTION_H
#include "clientexeption.h"

class ServerUnavailableException: public ClientExeption
{
public:
    ServerUnavailableException();
    ~ServerUnavailableException();
};

#endif // SERVERUNAVAILABLEEXCEPTION_H
