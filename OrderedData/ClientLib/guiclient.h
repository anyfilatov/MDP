#ifndef GUICLIENT_H
#define GUICLIENT_H

#include "abstractclient.h"
#include "Cache/rbtree.h"
#include "Exception/exception.h"
#include "typerequest.h"
#include "StatusCodes.h"

class GUIClient: AbstractClient
{
public:
    GUIClient(RBTree<QString, QString>* rbtree);
    QJsonObject getStatistics();
private:
    RBTree<QString, QString>* _rbtree;
};

#endif // GUICLIENT_H
