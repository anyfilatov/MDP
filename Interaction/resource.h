#pragma once
#include <QCoreApplication>
#include <vector>
#include "ip.h"

namespace integration {

    class Resource{
        std::vector<IP> address;
        QString owner;
    public:
        Resource(const QString& owner);

        IP PopResource(qint32 port);
        void SetResource();
    };

}
