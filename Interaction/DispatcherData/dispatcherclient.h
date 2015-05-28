#ifndef DISPATCHERCLIENT_H
#define DISPATCHERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QJsonArray>

class DispatcherClient : public QObject
{
    Q_OBJECT
public:
    explicit DispatcherClient(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort, QObject *parent = 0);
private:
    QTcpSocket* m_pTcpSocket;
    quint64     m_nNextBlockSize;
    QThread* thread;
    QString ip;
    QString spareIp;
    int port;
    int sparePort;
    void switchIps();
    int tryNum;
    QJsonObject slotReadyRead(QJsonObject &obj);
protected:
    void _connect();
public slots:
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void sendToServer(const QJsonObject& jso);
public:
    std::vector<QString> pingAll();
    void addHost(const QString& ip, int port);
    void setIps(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort);
};

#endif // DISPATCHERCLIENT_H
