#pragma once

#include <string>
#include "wrapper.h"
#include "host.h"
#include "DataBase.h"
#include "RbTree.h"


class Task
{
	int ID;
	QString Command;
    Wrapper<RbTree> RB3;
	Wrapper<DataBase> DB;

public:
    Task(const HostContent& content, Wrapper<RbTree> rb, Wrapper<DataBase> db);
	~Task(void);
    int operator()();
};


