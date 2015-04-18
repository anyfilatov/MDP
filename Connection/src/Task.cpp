#include "Task.h"
#include "Script.h"

Task::Task(const HostContent& command, wrapper<RbTree> rb, wrapper<DataBase> db)
    : ID( 0), Command(command.getData()), RB3 (rb), DB(db)
{}

Task::~Task(void)
{
}

int Task::operator()()
{
    Script script;
    char* code = "io.write('Hello world')";
    script.Create();
    script.DoString(code);
    script.Close();
	return 0;
}
