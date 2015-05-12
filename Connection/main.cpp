#include "LuaExecutor.h"
#include "Logger.h"
#include "Server.h"
#include <tuple>
#include <functional>
#include <list>
#include "JsonParser.h"

using namespace std;

int main(int argc, char ** argv) {
    using namespace std::placeholders;
    if(argc < 2) {
        std:: cout << "./server <id>" << std::endl;
        return 1;
    }
    char* confFileName = argv[1];
    int id = atoi(argv[2]);
    QString fileName(confFileName);
    DataBase dataBase;
    DB db(&dataBase);
    RbTree rbTree;
    RB rb(&rbTree);
    JsonParser parser(fileName);
    OG::WrappedType oG = parser.getHosts();
    Host currentHost;
    OG::WrappedType::iterator it = oG.begin();
    for( ; it != oG.end(); ++it) {
        auto& h = *it;
        if(h.getId() == id) {
            break;
        }
        LOG_DEBUG("all:" << h.getIP().toStdString() << ":" << h.getPort());
    }
    if(it == oG.end()) {
        LOG_DEBUG("Not found this host:" << id);
        return 1;
    }
    currentHost = *it;
    LOG_DEBUG("" << it->getIP().toStdString() << ":" << it->getPort());
    oG.erase(it);
    LOG_DEBUG("fileName=" << fileName.toStdString());
    OG og(&oG);
    Server server(currentHost.getIP(), currentHost.getPort(), db, rb, og);
//    OrGraph g(server);
    server.run();
    return 0;
}
