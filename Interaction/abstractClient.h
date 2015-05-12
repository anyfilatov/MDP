#pragma once
#include "ip.h"

namespace integration {
    class AbstractClient{
    public:
        //AbstractClient(){}

        virtual bool CheckHosts(IP&) = 0;
        virtual void SendConfig() = 0;
        virtual void UpdateConfig() = 0;
    };
}
