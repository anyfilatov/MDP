#ifndef CLIENT
#define CLIENT

#include <QObject>
#include <QString>
#include <QTcpSocket>

class Client: public QObject {
    Q_OBJECT
public:
    Client(QString host, int port, QObject *parent = 0);
    void put(QString key, QString value);
    QList<QString> get(QString key);
    int remove(QString key);
    QList<QPair<QString, QList<QString> > > getAll();
private:
    QTcpSocket *socket;
    QString host;
    int port;
};

#endif // CLIENT

