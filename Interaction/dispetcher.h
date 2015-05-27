#pragma once
#include "biGraph.h"
#include "abstractClient.h"
#include "resource.h"

namespace integration {
    class Dispetcher : public BiGraph<AbstractClient*, Resource>{
        bool carryOnChecking;
    public:
        Dispetcher(/*QTextEdit* logForRBT, QTextEdit* logForDisp, QTextEdit* logForBD*/);
        void CheckClients();
        void SendConfigToAll();
        void SendReinitializeMessage();
    };
}

