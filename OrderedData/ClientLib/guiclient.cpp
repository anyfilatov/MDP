#include "guiclient.h"

GUIClient::GUIClient(RBTree<QString, QString> *rbtree)
{
    _rbtree = rbtree;
}

QJsonObject GUIClient::getStatistics()
{
    QJsonObject req, resp;
    req.insert("type", STATS);
    write(req);
    resp = read();
    return resp;
}
