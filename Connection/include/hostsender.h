#ifndef HOSTSENDER
#define HOSTSENDER
#include "host.h"
#include "hostsender.h"

class HostSender
{
private:
    HostContent content;
public:
    HostSender(const HostContent& content):content(content){}

    void operator()(const Host& host){
          host.send(content);
    }
};
#endif // HOSTSENDER
