#pragma once

#include <map>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "Logger.h"

static const int SCHEDULER_STARTED = 0;
static const int SCHEDULER_STOPPED = 1;
static const int SCHEDULER_INTERRUPTED = 2;

template<class T>
class TaskScheduler {
    typedef std::multimap<int, T, std::greater_equal<int> > Queue;
    typedef std::mutex Mutex;
    typedef std::unique_lock<Mutex> WriteLock;
    typedef std::condition_variable Condition;
public:
    typedef T Task;

    TaskScheduler() : isStopped_(true) {
    };

    ~TaskScheduler() {
    };
    /**
     * start scheduler. 
     */
    void start();
    
    /**
     * stop scheduler
     */
    void stop();
    /**
     * interrupt scheduler
     */
    void interrupt();
    /**
     * check that scheduler stopped
     * @return true if stopped else false
     */
    bool isStopped()const;
    /**
     * 
     * @param task
     * @param priority
     * @return true if task added, else false
     */
    bool add(const T& task, int priority);
    /**
     * clear queue
     */
    void clear();
    /**
     * 
     * @return size of queue
     */
    size_t size();
    /**
     * 
     * @return true if queue empty else false
     */
    bool empty()const;

    bool waitEmptyQueue()const;
    
    /**
     * get task from queue
     * @return 
     */
    T get();
private:
    /**
     * notify all threads
     */
    void notifyAll();
private:
    std::atomic<int> isStopped_;
    Queue queue_;
    mutable Mutex lock_;
    Condition condition_;
};

template<class T>
void TaskScheduler<T>::start() {
    LOG_TRACE("queue start");
    isStopped_ = SCHEDULER_STARTED;
}

template<class T>
void TaskScheduler<T>::stop() {
    LOG_TRACE("queue stop");
	WriteLock lock(lock_);
    isStopped_ = SCHEDULER_STOPPED;
    notifyAll();//wake_up all threads
}

template<class T>
void TaskScheduler<T>::interrupt() {
	WriteLock lock(lock_);
	LOG_WARNING("queue interrupt");
    isStopped_ = SCHEDULER_INTERRUPTED;
    notifyAll();//wake_up all threads
}

template<class T>
bool TaskScheduler<T>::isStopped()const {
    return isStopped_ ;
}

template<class T>
bool TaskScheduler<T>::add(const T& task, int priority) {
    LOG_TRACE("add task");
    WriteLock lock(lock_);
    LOG_TRACE("add task after lock");
    queue_.insert(std::make_pair(priority, task));
    LOG_TRACE("notify one");
    condition_.notify_one();
    return true;
}

template<class T>
void TaskScheduler<T>::clear() {
    if (isStopped()) {
        WriteLock lock(lock_);
        queue_.clear();
    }
}

template<class T>
size_t TaskScheduler<T>::size() {
    LOG_TRACE("size");
    WriteLock lock(lock_);
    LOG_TRACE("size in lock");
    return queue_.size();
}

template<class T>
bool TaskScheduler<T>::empty()const {
    WriteLock lock(lock_);
    return queue_.empty();
}

template<class T>
void TaskScheduler<T>::notifyAll() {
    LOG_TRACE("notify all");
    condition_.notify_all();
}

template<class T>
T TaskScheduler<T>::get() {
    T task;
    WriteLock lock(lock_);
    LOG_TRACE("in lock");
    if (!isStopped() && queue_.empty()) {
        LOG_TRACE("wait");
        condition_.wait(lock);
    }
    if(isStopped_ != SCHEDULER_INTERRUPTED){
        if (!queue_.empty()) {
                typename Queue::iterator it = queue_.begin();
                LOG_DEBUG("get:" << it->first);
                task = it->second;
                queue_.erase(it);
        }
    }
    LOG_TRACE("return task");
    return task;
}



