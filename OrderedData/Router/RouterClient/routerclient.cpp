#include "routerclient.h"
#include "Exception/exception.h"

RouterClient::RouterClient() {

}

QJsonObject RouterClient::doRequestToOtherRouter(QJsonObject json, QString address, int port) {
    QJsonObject jsonResp;
    json.insert("not_forwards_requests", true);
    connectToHost(address, port);
    write(json);
    jsonResp = read();
    disconnectFromHost();
    return jsonResp;
}
