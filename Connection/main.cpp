#include "LuaExecutor.h"
#include "Logger.h"
#include "Server.h"
#include <tuple>
#include <functional>
#include <list>
#include "JsonParser.h"

using namespace std;
INST_LOGGGER();

void buildGraph(OrGraph<Host>& outOg, std::vector<Host>& hosts, std::map<int, std::vector<int>>& relations, int myId) {
    std::map<int, int> indexMap;
    for(auto& r : relations) {
        for(auto& id: r.second){
            indexMap[id] = r.first;
        }
    }
    auto findHost = [&](int id) -> int {
        for(size_t i = 0; i < hosts.size(); i ++) {
            if(hosts[i].getId() == id){
                return i;
            }
        }
        return -1;
    };
    bool haveMyId = false;
    for(auto& r : relations) {
        haveMyId = haveMyId || r.first == myId;
        if(!haveMyId ) {
            continue;
        }
        LOG_DEBUG("find " << r.first);
        auto* root = outOg.findIf([&](Host& host) -> bool {return host.getId() == r.first;});
        if(!root) {
            LOG_DEBUG("not found ");
            int index = findHost(r.first);
            LOG_DEBUG("index=" << index);
            if(index != -1){
                outOg.setRoot(hosts[index]);
                root = outOg.getRoot();
            } else {
                continue;
            }
        }
        for(auto& id : r.second){
            LOG_DEBUG("find " << id);
            int index = findHost(id);
            LOG_DEBUG("id index=" << id);
            if(index != -1){
                LOG_DEBUG("add index=" << index);
                root->addChild(hosts[index]);
            }
        }
    }
}

int main(int argc, char ** argv) {
    using namespace std::placeholders;
    if(argc < 2) {
        std:: cout << "./server <id>" << std::endl;
        return 1;
    }
    Logger::addConsoleLogger();
    char* confFileName = argv[1];
    int id = atoi(argv[2]);
    QString fileName(confFileName);
    DataBase dataBase;
    DB db(&dataBase);
    RbTree rbTree;
    RB rb(&rbTree);
    JsonParser parser(fileName);
    auto hosts = parser.getHosts();
    auto relations = parser.readRelations();
    OrGraph<Host> org;
    buildGraph(org, hosts, relations, id);
    if(org.empty()) {
        LOG_FATAL("Orgraph is emoty");
        return -1;
    }
    Host currentHost;
    for(auto it = hosts.begin(); it != hosts.end(); ++it) {
        if(it->getId() == id){
            currentHost = *it;
            hosts.erase(it);
            break;
        }
    }
    OG og(&org);
    Server server(currentHost.getIP(), currentHost.getPort(), db, rb, og, fileName);
//    OrGraph g(server);
    server.run();
    return 0;
}
