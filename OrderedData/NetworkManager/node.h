#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>

class NetworkManager;

class Node: public QObject {
    Q_OBJECT
public:
    Node();
    Node(QString host, qint16 port, qint32 heartbeat, qint32 cleanupInterval, NetworkManager& manager);
    Node(QString host, qint16 port, qint32 heartbeat);
    Node(const Node& orig);
    QString getHost() const;
    qint16 getPort() const;
    qint32 getHeartbeat() const;
    QString getAddress() const;
    const QTimer* getTimer() const;
    void setHeartbeat(qint32 heartbeat);
    void startTimer();
    QJsonObject& toJsonObject() const;
    Node& operator=(const Node& other);
    bool operator==(const Node& other) const;
    bool operator<(const Node& other) const;
    virtual ~Node();
signals:
    void heartbeatChanged();
    void notifyManager(QString address);
public slots:
    void handleHeartbeatChanged();
    void handleTimeout();
protected:
    QString _host;
    QTimer _timer;
    qint16 _port;
    qint32 _heartbeat;
};

#endif // NODE_H
