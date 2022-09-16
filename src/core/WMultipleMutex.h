#pragma once

#include "utils/common_script.h"
#include <pthread.h>
#include <semaphore.h>

class WMultipleMutex
{
private:
    sem_t *sem;
    int size;
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
    return this->block(0, size - 1);
}

force_inline int WMultipleMutex::block(int from, int to)
{
    for(; from < to; from ++){
        if(unlikely(sem_trywait(&sem[to]))){
            return -WMULTIMUTEX_ALREADY_LOCK;
        }
    }

    return 0;
}

force_inline int WMultipleMutex::unlock(int from, int to)
{
    W_ASSERT(to <= size);
    W_ASSERT(from >= 0);

    for(; from < to; from ++){
        sem_post(&sem[from]);
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

