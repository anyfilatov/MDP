#include "connectionisnotcreatedexception.h"

ConnectionIsNotCreatedException::ConnectionIsNotCreatedException()
{
    init(ExceptionCode::CONNECTION_IS_NOT_CREATED);
}

ConnectionIsNotCreatedException::~ConnectionIsNotCreatedException()
{

}

