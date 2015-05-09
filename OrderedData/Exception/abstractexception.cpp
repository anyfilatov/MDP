#include "abstractexception.h"

QString AbstractException::getMessage()
{
    return msg;
}

QString AbstractException::printError(){
    switch (error) {
    case SERVER_UNAVAILABLE:
        return "SERVER_UNAVAILABLE";
    default:
        return "CLIENT_ERROR";
    }
}

AbstractException::AbstractException(){

}

AbstractException::~AbstractException(){

}
