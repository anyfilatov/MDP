#ifndef ABSTRACTEXCEPTION_H
#define ABSTRACTEXCEPTION_H
#include <QtCore>
#include "exceptioncode.h"
class AbstractException
{
public:
    AbstractException();
    ~AbstractException();
    virtual QString getMessage();
    virtual QString printError();

protected:
    QString msg;
    ExceptionCode error;
};

#endif // ABSTRACTEXCEPTION_H
