#include "serverunavailableexception.h"
ServerUnavailableException::ServerUnavailableException()
{
    init(ExceptionCode::SERVER_UNAVAILABLE_EXEPTION);
}

ServerUnavailableException::~ServerUnavailableException()
{

}

