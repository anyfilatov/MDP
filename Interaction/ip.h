#pragma once
#include <QCoreApplication>
namespace integration{
    class IP{
        QString ip;
        qint32 port;
    public:
        IP(const QString& ip = "",const qint32& port = 0):ip(ip),port(port){}

        QString GetIP();
        qint32 GetPort();
        QString GetFullAddress();
        void SetFullAddress(const QString& ip, const qint32& port);
    };
}
