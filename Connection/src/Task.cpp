#include "Task.h"

Task::Task(const HostContent& command, Wrapper<RbTree> rb, Wrapper<DataBase> db)
    : ID( 0), Command(command.getData()), RB3 (rb), DB(db)
{}

Task::~Task(void)
{
}

int Task::operator()()
{
    return 0;
}
