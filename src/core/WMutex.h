#ifndef WMUTEX_H
#define WMUTEX_H

#include "utils/common_script.h"
#include <pthread.h>

class WMutex
{
private:
    pthread_mutex_t _m;
public:
    WMutex();
    ~WMutex();
    void lock();
    void unlock();
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
    pthread_mutex_init(&_m, NULL);
}


#endif // WMUTEX_H
