//
// Created by imelker on 10.01.2021.
//
#include "ThreadPool.h"

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        this->workers.emplace_back([this] () {
            for (;;) {
               std::function<void()> task;
               {
                   std::unique_lock<std::mutex> lock(this->mutex);
                   this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                   if (this->stop && this->tasks.empty())
                       return;
                   task = std::move(this->tasks.front());
                   this->tasks.pop();
               }
               task();
            }
        });
    }
}

// the destructor joins all threads
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->stop = true;
    }
    this->condition.notify_all();
    for (std::thread &worker: this->workers)
        worker.join();
}
