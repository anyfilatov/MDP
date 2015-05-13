#ifndef CLIENT
#define CLIENT

#include <QString>
#include <QTcpSocket>
#include <QSettings>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHostAddress>
#include "Router/typerequest.h"
#include "Router/StatusCodes.h"

class Client : public QObject {
  Q_OBJECT
 public:
  explicit Client(QString settingsFileName, QObject* parent = 0);

  int put(QString key, QString value, QString bucket = NULL);
  int put(QString key, QStringList values, QString bucket = NULL);
  int replace(QString key, QStringList values, QString bucket = NULL);

  QStringList get(QString key, QString bucket = NULL);

  int remove(QString key);
  int removeBucket(QString bucket);

  QStringList getRingHosts();
  void joinToRing(QString who, QStringList ring);

 protected:
  Client(QObject* parent = 0);
  QTcpSocket* _socket = NULL;
  QStringList _hosts;

  void writeMsg(QJsonObject msg);
  QJsonObject readMsg();
  QJsonObject deserialize(QByteArray data);
  QByteArray serialize(QJsonObject json);

  int openConnection();

 protected slots:
  void disconnected();
};

#endif  // CLIENT
