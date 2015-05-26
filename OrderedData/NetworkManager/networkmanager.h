#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QList>
#include <QMutex>

#include "node.h"
#include "NetworkSettings.h"
#include "SendGossipTask.h"
#include "RecieveGossipTask.h"

class NetworkManager : public QObject {
  Q_OBJECT
 public:
  static const int MAX_PACKET_LENGTH = 102400;
  NetworkManager(NetworkSettings& settings);
  ~NetworkManager();
  QList<Node*>& getMemberList();
  void addMembers(QStringList nodes);
  QList<Node*> getAllMembers();
  QList<Node*>& getDeadNodes();
  Node* getMyself();
  const NetworkSettings* getSettings() const;
 public slots:
  void handleDeadNode(QString address);

 private:
  QList<Node*> _memberList;
  QList<Node*> _deadList;
  Node* _me;
  NetworkSettings* _settings;
  SendGossipTask* _sendGossipTask;
  RecieveGossipTask* _recieveGossipTask;
  QMutex _mutex;
};

#endif  // NETWORKMANAGER_H
