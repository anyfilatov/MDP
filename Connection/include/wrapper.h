#pragma once

#include <mutex>

template <class T>
class wrapper
{
	T* object;
	std::mutex* m;

public:

	wrapper(T* o)
	{
            object = o;
            m = new std::mutex();
	}

	~wrapper(void)
	{
            if(m){delete m;}
	}
	
	void lock()
	{	
        m->lock();
	}

	void unlock()
	{
        m->unlock();
	}

	T* operator->() const
	{
		return object;
	}
};


