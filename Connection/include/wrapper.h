#pragma once

#include <mutex>
#include <memory>
#include "Logger.h"

template <class T>
class Wrapper {
    T* object;
    std::shared_ptr<std::mutex> m;

public:
    typedef T WrappedType;

    class ScopedLock {
    public:

        ScopedLock(Wrapper<T> t) : wrp_(t) {
            LOG_TRACE("ScopedLock");
            wrp_.lock();
            LOG_TRACE("ScopedLock lock");
        }

        ~ScopedLock() {
            LOG_TRACE("~ScopedLock");
            wrp_.unlock();
            LOG_TRACE("ScopedLock unlock");
        }
    private:
        Wrapper<T> wrp_;
    };

    Wrapper(T* o) {
        LOG_TRACE("new Wrapper");
        object = o;
        m = std::make_shared<std::mutex>();
    }

    Wrapper(const Wrapper<T>& w) {
        object = w.object;
        m = w.m;
    }

    ~Wrapper(void) {
    }

    void lock() {
        LOG_TRACE("lock")
        m->lock();
    }

    void unlock() {
        LOG_TRACE("unlock")
        m->unlock();
    }

    T* operator->() const {
        return object;
    }
};


