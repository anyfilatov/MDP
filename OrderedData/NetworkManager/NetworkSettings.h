#ifndef NETWORKSETTINGS_H
#define	NETWORKSETTINGS_H

#include <QList>
#include <QPair>
#include <QFile>

#define DEFAULT_PORT 35980
#define DEFAULT_GOSSIP_INTERVAL 1000
#define DEFAULT_CLEANUP_INTERVAL 10000

class NetworkSettings {
public:
    NetworkSettings(QString host, qint32 port, qint32 gossipInterval, qint32 cleanupInterval);
    NetworkSettings(QFile& settigs);
    NetworkSettings(NetworkSettings& orig);
    virtual ~NetworkSettings();
    
    QList<QPair<QString, qint16> > getGossipNodes();
    
    void setCleanupInterval(qint32 _cleanupInterval);
    qint32 getCleanupInterval() const;
    
    void setGossipInterval(qint32 _gossipInterval);
    qint32 getGossipInterval() const;
    
    void setPort(qint32 _port);
    qint32 getPort() const;
    
    void setHost(QString host);
    QString getHost() const;
    
private:
    QString _host;
    qint32 _port;
    qint32 _gossipInterval;
    qint32 _cleanupInterval; 
    QList<QPair<QString, qint16> > _gossipNodes;
};

#endif	/* NETWORKSETTINGS_H */

