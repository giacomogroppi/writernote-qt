#pragma once

#include "utils/WCommonScript.h"
#include <pthread.h>
#include "WSemaphore.h"

class WMultipleMutex
{
private:
    WSemaphore *_sem;
    int _size;
public:
    WMultipleMutex(int maxVal, int initVal);
    ~WMultipleMutex();

    // to is not included
    int block(int from, int to);
    int blockAll();

    int unlock(int from, int to);

    int lock(int n);
    int unlock(int n);
};


#define WMULTIMUTEX_ALREADY_LOCK 1
force_inline int WMultipleMutex::blockAll()
{
    return this->block(0, _size - 1);
}

force_inline int WMultipleMutex::block(int from, int to)
{
    for(; from < to; from ++){
        if (un(_sem[0].tryWait())) {
            return -WMULTIMUTEX_ALREADY_LOCK;
        }
    }

    return 0;
}

force_inline int WMultipleMutex::unlock(int from, int to)
{
    W_ASSERT(to <= _size);
    W_ASSERT(from >= 0);

    for(; from < to; from ++){
        _sem[from].release();
    }

    return 0;
}

// it means that the last thread is trying to free
// the last one its successes, which does not
// exist, so we return only 0
force_inline int WMultipleMutex::unlock(int n)
{
    return this->unlock(n, n+1);
}

