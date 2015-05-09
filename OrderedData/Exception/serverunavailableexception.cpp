#include "serverunavailableexception.h"
ServerUnavailableException::ServerUnavailableException()
{
    init(ExceptionCode::SERVER_UNAVAILABLE);
}

ServerUnavailableException::~ServerUnavailableException()
{

}

