#include "routerclient.h"
#include "ClientLib/typerequest.h"
#include "Exception/exception.h"

RouterClient::RouterClient() {

}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, int port) {
    QJsonObject jsonResp;
    json.insert("not_forwards_request", true);
//    qDebug() << "Going to send to router " << address << ":" << port;
    connectToHost(address, port);
    write(json);

    if (json["type"] == BATCH) {
        disconnectFromHost();
        return jsonResp;
    } else {
        jsonResp = read();
        disconnectFromHost();
        return jsonResp;
    }
}
