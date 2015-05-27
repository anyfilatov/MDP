#pragma once
#include "ip.h"
#include <QThread>
#include <QString>
#include <QTextEdit>

namespace integration {
    class AbstractClient{
    public:
        //AbstractClient(){}

        virtual bool CheckHosts(IP&, std::vector<QString> &) = 0;
        virtual void SendConfig(IP &, std::vector<QString> &) = 0;
        virtual void UpdateConfig() = 0;
        virtual QString getName() = 0;

        friend std::ostream& operator << (std::ostream& ostr, AbstractClient& ac){
            ostr << ac.getName().toStdString();
            return ostr;
        }

    };
}
