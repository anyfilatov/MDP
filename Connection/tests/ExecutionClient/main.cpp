/*
 * File:   main.cpp
 * Author: savinov
 *
 * Created on May 12, 2015, 5:52 PM
 */

#include <QApplication>
#include "ExecutionClient.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);
    Execution::Client client("127.0.0.1", 4001);
    if( client.ping().empty() ) {
        std::cout << "good" << std::endl;
    } else {
        std::cout << "bad" << std::endl;
    }
    client.setNewChild(std::vector<QString>());
    return 0;
}
