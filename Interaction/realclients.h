#pragma once
#include "abstractClient.h"
namespace integration {
    class RealClientBD : public AbstractClient {//, clientBD {
    public:
        bool CheckHosts(IP &);
        void SendConfig();
        void UpdateConfig();
    };

    class RealClientRBT : public AbstractClient {//, clientRBT {
    public:
        bool CheckHosts(IP &);
        void SendConfig();
        void UpdateConfig();
    };

    class RealClientDisp : public AbstractClient {//, clientDisp {
    public:
        bool CheckHosts(IP &);
        void SendConfig();
        void UpdateConfig();
    };
}

