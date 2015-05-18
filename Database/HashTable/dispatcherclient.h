#ifndef DISPATCHERCLIENT_H
#define DISPATCHERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class DispatcherClient : public QObject
{
    Q_OBJECT
public:
    explicit DispatcherClient(const QString& strHost, int nPort, QObject *parent = 0);
private:
    QTcpSocket* m_pTcpSocket;
    quint64     m_nNextBlockSize;
public slots:
    QJsonObject slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void sendToServer(const QJsonObject& jso);
public:
    vector<QString> pingAll();
    void addHost(const QString& ip, int port);
};

#endif // DISPATCHERCLIENT_H
