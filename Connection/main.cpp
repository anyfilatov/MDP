#include "LuaExecutor.h"
#include "Logger.h"
#include <tuple>
#include <functional>
#include <list>

using namespace std;

int main(int, char *[]) {
    using namespace std::placeholders;
    DataBase dataBase;
    DB db(&dataBase);
    RbTree rbTree;
    RB rb(&rbTree);
    try{
        LuaExecutor executor(db, rb, "script.lua", true);
        executor.execute();
    } catch (const std::exception& e) {
        LOG_ERROR("exception:" << e.what());
    }
    return 0;
}
