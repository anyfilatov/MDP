#include "notfoundvalueexception.h"

NotFoundValueException::NotFoundValueException()
{
    init(ExceptionCode::NOT_FOUND_VALUE_EXCEPTION);
}

NotFoundValueException::~NotFoundValueException()
{

}

