/*
 * File:   main.cpp
 * Author: savinov
 *
 * Created on March 25, 2015, 10:37 PM
 */


#include "Server.h"
//#include "LuaExecutor.h"

int main(int , char **) {
    
    Server server("localhost", 5000);

    return server.Run();
//    LuaExecutor e;
//    e.exec();
    //return 0;
}
