#ifndef CLIENT
#define CLIENT

#include <QString>
#include <QTcpSocket>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHostAddress>
#include "Router/typerequest.h"

class Client : public QObject {
  Q_OBJECT
 public:
  explicit Client(QObject* parent = 0);

  int put(QString key, QString value, QString bucket = NULL);
  int put(QString key, QStringList values, QString bucket = NULL);
  int replace(QString key, QStringList values, QString bucket = NULL);

  QStringList get(QString key, QString bucket = NULL);

  int remove(QString key);
  int removeBucket(QString bucket);

  QStringList getRingHosts();
  void joinToRing(QString who, QStringList ring);

 protected:
  QTcpSocket* _socket = NULL;
  QList<QString> _hosts;

  void writeMsg(QJsonObject msg);
  QJsonObject readMsg();
  QJsonObject deserialize(QByteArray data);
  QByteArray serialize(QJsonObject json);

  int openConnection();

 protected slots:
  void disconnected();
};

#endif  // CLIENT
