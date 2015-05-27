#include <QByteArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QHostAddress>

#include "abstractclient.h"
#include "Exception/serverunavailableexception.h"

AbstractClient::AbstractClient()
{
    _socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
}

AbstractClient::~AbstractClient()
{

}

bool AbstractClient::connectToHost(const QString& host, quint16 port)
{
    _socket.connectToHost(QHostAddress(host), port);
    return _socket.waitForConnected(AbstractClient::MAX_WAIT_TIME_MLS);
}

void AbstractClient::write(const QJsonObject &object)
{
    QByteArray rawData = QJsonDocument(object).toBinaryData();

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << (quint32)0;
    out << rawData;
    out.device()->seek(0);
    out << (quint32)(packet.size() - sizeof(quint32));

    if (_socket.isWritable()) {
        _socket.write(packet);
        _socket.waitForBytesWritten(AbstractClient::MAX_WAIT_TIME_MLS);
        _socket.flush();
    }
}

QJsonObject AbstractClient::read()
{
    QByteArray response;
    quint32 packetSize = 0;

    QDataStream in(&_socket);
    in.setVersion(QDataStream::Qt_5_0);

    if (_socket.waitForReadyRead(AbstractClient::MAX_WAIT_TIME_MLS)) {
        while (_socket.bytesAvailable() < sizeof(quint32)) {
            if (!_socket.waitForReadyRead(AbstractClient::MAX_WAIT_TIME_MLS)) {
                throw ServerUnavailableException();
            }
        }

        in >> packetSize;

        QByteArray packet;

        while (_socket.bytesAvailable() < sizeof(quint32)) {
            if (!_socket.waitForReadyRead(AbstractClient::MAX_WAIT_TIME_MLS)) {
                throw ServerUnavailableException();
            }
        }
        packet.append(_socket.read(packetSize));

        QDataStream packetStream(&packet, QIODevice::ReadOnly);

        packetStream >> response;

        return QJsonDocument::fromBinaryData(response).object();

    } else {
        qDebug() << "Wait return false. Can't recieve packet";
        throw ServerUnavailableException();
    }
}

void AbstractClient::disconnectFromHost()
{
    _socket.waitForBytesWritten();
    _socket.disconnectFromHost();
}

