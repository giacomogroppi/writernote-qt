#ifndef WMULTIPLEMUTEX_H
#define WMULTIPLEMUTEX_H

#include "utils/common_script.h"
#include <pthread.h>
#include <semaphore.h>

class WMultipleMutex
{
private:
    pthread_mutex_t *mutex;
    int maxVal;
public:
    WMultipleMutex(int maxVal, int initVal);
    ~WMultipleMutex();

    void blockAll();

    int lock(int n);
    int unlock(int n);
};

force_inline void WMultipleMutex::blockAll()
{
    int i = maxVal;
    for(i--; i >= 0; i--){
        pthread_mutex_lock(&mutex[i]);
    }
}

#endif // WMULTIPLEMUTEX_H
