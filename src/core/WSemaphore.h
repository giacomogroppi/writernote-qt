#pragma once

#include "utils/WCommonScript.h"

#include <iostream>
#include <semaphore>
#include <mutex>

class WSemaphore {
private:
#if __cplusplus > 201703L
    std::binary_semaphore _sem;
#else
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.
#endif // C20
public:
    explicit WSemaphore(int init_value = 0);
    ~WSemaphore();

    WDISABILE_COPY(WSemaphore);

    bool tryWait();
    void acquire();
    void release();
    void release(int n);
};

inline WSemaphore::WSemaphore(int init_value)
#if __cplusplus > 201703L
    : _sem(init_value)
#else
    : count_(0)
#endif
{
}

inline WSemaphore::~WSemaphore() = default;

inline void WSemaphore::acquire()
{
#if __cplusplus > 201703L
    _sem.acquire();
#else
    std::unique_lock<decltype(mutex_)> lock(mutex_);
    while(!count_) // Handle spurious wake-ups.
        condition_.wait(lock);
    --count_;
#endif
}

inline void WSemaphore::release()
{
#if __cplusplus > 201703L
    _sem.release();
#else
    std::lock_guard<decltype(mutex_)> lock(mutex_);
    ++count_;
    condition_.notify_one();
#endif
}

inline bool WSemaphore::tryWait()
{
#if __cplusplus > 201703L
    return this->_sem.try_acquire();
#else
    std::lock_guard<decltype(mutex_)> lock(mutex_);
    if(count_) {
        --count_;
        return true;
    }
    return false;
#endif
}

inline void WSemaphore::release(int n)
{
    for (int i = 0; i < n; i++)
        release();
}
