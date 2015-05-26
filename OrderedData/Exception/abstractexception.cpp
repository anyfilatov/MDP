#include "abstractexception.h"

QString AbstractException::getMessage()
{
    return msg;
}

QString AbstractException::printError(){
    switch (error) {
    case SERVER_UNAVAILABLE_EXEPTION:
        return "SERVER_UNAVAILABLE";
    case NOT_FOUND_VALUE_EXCEPTION:
        return "NOT_FOUND_VALUE_EXCEPTION";
    case CONNECTION_IS_NOT_CREATED:
        return "CONNECTION_IS_NOT_CREATED";
    default:
        return "CLIENT_ERROR";
    }
}

AbstractException::AbstractException(){

}

AbstractException::~AbstractException(){

}
