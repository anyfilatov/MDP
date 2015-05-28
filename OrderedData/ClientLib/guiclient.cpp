#include "guiclient.h"

GUIClient::GUIClient(QString settingsFileName)
{
    QFile settingsFile(settingsFileName);
    if (!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Couldn't open file");

    QJsonParseError* parseError = NULL;
    QJsonDocument document =
            QJsonDocument::fromJson(settingsFile.readAll(), parseError);
    if (parseError == NULL) {
        QJsonObject json = document.object();
        QJsonArray members = json.value("members").toArray();
        for (int i = 0; i < members.size(); i++) {
            QString member = members.at(i).toString();
            _hosts.append(member);
        }
    } else {
        throw std::runtime_error("Error while parsing settings file");
    }
    settingsFile.close();
}

QJsonArray GUIClient::getJSONData()
{
    QJsonArray response;
    for(QString host: _hosts){
        QJsonObject obj = getStatistics(host);
        obj.insert("host", host);
        response.append(QJsonValue(obj));
    }
    return response;
}

QJsonObject GUIClient::getStatistics(QString host)
{
    QJsonObject req, resp;
    req.insert("type", STATS);
    connectToHost(host.split(':').first(), host.split(':').back().toInt());
    write(req);
    resp = read();
    disconnectFromHost();
    return resp;
}
