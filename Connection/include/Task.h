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
    wrapper<RbTree> RB3;
	wrapper<DataBase> DB;

public:
    Task(const HostContent& content, wrapper<RbTree> rb, wrapper<DataBase> db);
	~Task(void);
    int operator()();
};


