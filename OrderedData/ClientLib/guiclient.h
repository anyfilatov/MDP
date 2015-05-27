#ifndef GUICLIENT_H
#define GUICLIENT_H

#include <QFile>
#include <QJsonArray>
#include "abstractclient.h"
#include "Cache/rbtree.h"
#include "Exception/exception.h"
#include "typerequest.h"
#include "StatusCodes.h"

class GUIClient: AbstractClient
{
public:
    GUIClient(QString settingsFileName);
    QJsonArray getJSONData();
private:
    QJsonObject getStatistics(QString host);
    QStringList _hosts;
};

#endif // GUICLIENT_H
