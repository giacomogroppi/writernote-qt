#pragma once

#include "utils/WCommonScript.h"
#include <pthread.h>

class WMutex
{
private:
    mutable pthread_mutex_t _m{};
public:
    WMutex();

    WMutex(const WMutex &other) = delete;
    WMutex(WMutex &&other) = delete;

    WMutex &operator=(const WMutex &other) = delete;

    ~WMutex();
    void lock();
    void unlock();
    bool isLocked() const;
};

force_inline void WMutex::lock()
{
    pthread_mutex_lock(&_m);
}

force_inline void WMutex::unlock()
{
    pthread_mutex_unlock(&_m);
}

force_inline WMutex::~WMutex()
{
    pthread_mutex_destroy(&_m);
}

force_inline WMutex::WMutex()
{
    pthread_mutex_init(&_m, nullptr);
}

inline bool WMutex::isLocked() const
{
    if (pthread_mutex_trylock(&_m) != 0) {
        return true;
    } else {
        pthread_mutex_unlock(&_m);
        return false;
    }
}

