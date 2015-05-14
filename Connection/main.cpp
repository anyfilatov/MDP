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
    if(argc < 3) {
        std:: cout << "./server <configFile> <id>" << std::endl;
        return 1;
    }
    try{
        Logger::addConsoleLogger();
        char* confFileName = argv[1];
        int id = atoi(argv[2]);
        QString fileName(confFileName);
    //    getHosts
        JsonParser parser(fileName);
        auto hosts = parser.getHosts("hosts");
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
        hosts = parser.getHosts("db");
        if(hosts.size() != 2) {
            LOG_ERROR("db hosts != 2");
            return 1;
        }
        DataBase dataBase(hosts[0].getIP(), hosts[0].getPort(), hosts[1].getIP(), hosts[1].getPort());
        hosts = parser.getHosts("db");
        if(hosts.size() < 1) {
            LOG_ERROR("no rb hosts");
            return 1;
        }
        hosts = parser.getHosts("rb");
        DB db(&dataBase);
        auto ip = hosts[0].getIP();
        LOG_DEBUG("ip=" << ip.toStdString());
        RbTree rbTree(hosts[0].getIP());
        RB rb(&rbTree);

        OG og(&org);
        Server server(currentHost.getIP(), currentHost.getPort(), db, rb, og, fileName);
    //    OrGraph g(server);
        server.run();
    } catch( AbstractException& e) {
        LOG_ERROR("Exception:" << e.getMessage().toStdString());
    } catch (const std::exception& ex) {
        LOG_ERROR("Exception:" << ex.what());
    }
    return 0;
}
