#include "clientexeption.h"

ClientExeption::ClientExeption()
{
    init((ExceptionCode)0, false);
}

ClientExeption::~ClientExeption()
{

}

void ClientExeption::init(ExceptionCode exp, bool children){
    msg = "Client exeption.";
    if (!children){
        return;
    }
    error = exp;
    msg.append(" Code: ");
    msg.append(printError());
}


