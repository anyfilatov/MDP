#ifndef SESSION
#define SESSION

#include "Serializible.h"

using namespace std;

class Session:public Serializible{
    private:
        int hostCount;
        int stringsCount;
        int increment;
        long lastModified;
    public:
        Session(int hosts, int stringsCount);
        ~Session();
        pair<int, int> getNextStringNumber(int count);
        QString serialize();
        void parse(QString json);
};

#endif // SESSION

