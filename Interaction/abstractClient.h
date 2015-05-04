#pragma once
#include "ip.h"

namespace integration {
    class AbstractClient{
    public:
        //AbstractClient(){}

        virtual bool CheckHosts(IP&){return true;}// = 0;
        virtual void SendConfig(){}// = 0;
        virtual void UpdateConfig(){}// = 0;
    };
}
