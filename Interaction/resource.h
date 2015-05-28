#pragma once
#include <QCoreApplication>
#include <vector>
#include "ip.h"
#include "iostream"

namespace integration {

    class Resource{
        std::vector<IP> address;
        QString owner;
    public:
        Resource(const QString& owner);

        IP popResource(qint32 port);
        void setResource();
        std::vector<IP> &getAddresses();
        bool operator == (Resource &tmp) const;
        bool operator != (Resource & tmp) const;
        QString getOwner();

        friend std::ostream& operator << (std::ostream& , Resource& ) ;

    };
    std::ostream& operator << (std::ostream& , Resource& ) ;
}
