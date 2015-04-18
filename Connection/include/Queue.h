#pragma once

#include <iostream>
#include <queue>
#include <deque>
#include <mutex>
#include <condition_variable>

template<class T> class TaskQueue{
public:
    typedef T Task;
    std::queue<T> q;
    std::mutex Mutex;
    std::condition_variable conditionalVariable;
    std::atomic<bool> isStopped_;
    void start(){
        std::cout<<" Start"<<std::endl;
        std::unique_lock<std::mutex> lock(Mutex);
        isStopped_ = false;
    };
    void stop(){ 
        std::unique_lock<std::mutex> lock(Mutex);
        std::cout<<" Stop"<<std::endl;
        isStopped_ = true;
        conditionalVariable.notify_all();
    };
    bool add(T element){
        std::unique_lock<std::mutex> lock(Mutex);
        if(!isStopped_){
            q.push(element);
            //change condition_variable, send a signal to one thread
            conditionalVariable.notify_one();
            return true;
        }else return false;
    };
    int size(){
        std::unique_lock<std::mutex> lock(Mutex);
        return q.size();
    };
    T get(){
        std::unique_lock<std::mutex> lock(Mutex);
        if(q.empty()){
            //wait a conditional signal
            conditionalVariable.wait( lock );
        }
        if(!q.empty()){
            T ret = q.back();
            q.pop();
            return ret;
        }
        return T();
    };
    bool isStopped(){
        return isStopped_;
    }
};

