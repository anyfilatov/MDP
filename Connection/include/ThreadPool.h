#pragma once

#include <map>
#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include <atomic>
#include <QException>
#include "Queue.h"
#include "Logger.h"
#include "abstractexception.h"

static const int DEFAULT_PRIORITY = 0;
static const int SLEEP_MSEC_IN_WAIT_LIMIT = 100;
static const int SLEEP_SEC_IN_WAIT_EMPTY_QUEUE = 1;
static const int MIN_THREADS_COUNT = 1;
static const int MIN_LIMIT = 1;

template<class TTask>
class ThreadPool {
private:

    template<class TQueue>
    class ThreadFunctor {
    public:

        ThreadFunctor(std::shared_ptr<TQueue> t) : queue_(t), isStopped_(false) {
        };

        ThreadFunctor(const ThreadFunctor& t) : queue_(t.queue_), isStopped_(t.isStopped_.load()) {
        };

        ~ThreadFunctor() {
        };
        void operator()();

        void stop() {
            isStopped_ = true;
        }

        bool isStopped()const {
            return isStopped_;
        }
    private:
        std::shared_ptr<TQueue> queue_;
        std::atomic<int> isStopped_;
    };

private:
    struct ThreadKeeper;
    typedef std::mutex Mutex;
    typedef std::unique_lock<Mutex> WriteLock;
    typedef std::thread Thread;
    typedef std::shared_ptr<Thread> ThreadPtr;
    typedef std::shared_ptr<TTask> TaskSharedPtr;
    typedef std::map<std::thread::id, ThreadKeeper> ThreadContainer;
    typedef TaskQueue<TaskSharedPtr> Queue;
    typedef std::shared_ptr<Queue> QueueSharedPtr;
    typedef std::shared_ptr<ThreadFunctor<Queue> > ThreadFunctorSharedPtr;

    struct ThreadKeeper {
        ThreadPtr thread;
        ThreadFunctorSharedPtr functor;
    };
public:
    ThreadPool(int threadCount);

    ~ThreadPool();
    bool start();

    bool isStopped()const {
        return isStopped_;
    }
    bool add(const TTask& task);
    void stop();
private:
    ThreadPool(const ThreadPool& orig);
    void clear();
    void waitThreads(std::function<void(ThreadPtr) > function);

    static void join(ThreadPtr thread) {
        if (thread) {
            thread->join();
        }
    }
private:
    int threadCount_;
    Mutex lock_;
    ThreadContainer threads_;
    QueueSharedPtr queue_;
    std::atomic<int> isStopped_;
};

template<class TTask>
ThreadPool<TTask>::ThreadPool(int threadCount) : threadCount_(threadCount)
, queue_(new typename QueueSharedPtr::element_type()), isStopped_(true) {
}

template<class TTask>
bool ThreadPool<TTask>::start() {
    WriteLock lock(lock_);
    if (threadCount_ < MIN_THREADS_COUNT) {
        return false;
    }
    isStopped_ = false;
    queue_->start();
    LOG_DEBUG("threadCount=" << threadCount_);
    for (size_t i = 0; i < threadCount_; i++) {
        ThreadKeeper keeper;
        keeper.functor = ThreadFunctorSharedPtr(new ThreadFunctor<Queue>(queue_));
        keeper.thread = ThreadPtr(new Thread(*keeper.functor));
        if (keeper.thread) {
            threads_.insert(std::make_pair(keeper.thread->get_id(), keeper));
        }
    }
    return true;
}

template<class TTask>
bool ThreadPool<TTask>::add(const TTask& task) {
    WriteLock lock(lock_);
    if (isStopped()) {
        return false;
    }
    LOG_TRACE("add task");
    TaskSharedPtr taskPtr(new typename TaskSharedPtr::element_type(task));
    return queue_->add(taskPtr);
}

template<class TTask>
void ThreadPool<TTask>::stop() {
    if (isStopped()) {
        return;
    }
    isStopped_ = true;
    queue_->stop();
    std::function<void(ThreadPtr) > func = std::bind(&ThreadPool<TTask>::join, std::placeholders::_1);
    waitThreads(func);
    threads_.clear();
}

template<class TTask>
void ThreadPool<TTask>::waitThreads(std::function<void(ThreadPtr) > function) {
    bool lastIdSet = false;
    std::thread::id lastId;
    while (true) {
        ThreadPtr thread;
        std::thread::id currentId;
        {
            WriteLock lock(lock_);
            typename ThreadContainer::iterator it = lastIdSet ? threads_.upper_bound(lastId) : threads_.begin();
            if (it != threads_.end()) {
                currentId = it->first;
                thread = it->second.thread;
                lastId = currentId;
                lastIdSet = true;
            } else {
                break;
            }
        }
        if (thread) {
            function(thread);
        }
    }
}

template<class TTask>
ThreadPool<TTask>::~ThreadPool() {
    stop();
};

template<class TTask>
void ThreadPool<TTask>::clear() {
    if (isStopped()) {
        queue_->clear();
    }
}

template<class TTask>
template<class TQueue>
void ThreadPool<TTask>::ThreadFunctor<TQueue>::operator()() {
    LOG_DEBUG("I'm started:" << std::this_thread::get_id());
    if (!queue_) {
        LOG_ERROR("Queue is null. Exit...");
    }
    isStopped_ = false;
    while (!isStopped()) {
        LOG_TRACE("queue_->get()");
        auto t = queue_->get();
        LOG_TRACE("Start task");
        try {
            if (t) {
                (*t)();
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Exception:" << e.what());
        } catch ( AbstractException& e) {
            LOG_ERROR("Exception:" << e.getMessage().toStdString());
        } catch (const QException& e) {
            LOG_ERROR("Exception:" << e.what());
        } catch (...) {
            LOG_ERROR("Exception:unknown");
        }
        LOG_TRACE("task ended");
    }
}

