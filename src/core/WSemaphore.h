#pragma once

#ifdef MACOS
#include <semaphore>
#else
# include "semaphore.h"
#endif

class WSemaphore {
private:
#ifdef MACOS
    std::counting_semaphore<0> _sem;
#else
    sem_t _sem;
#endif
public:
    WSemaphore(int init_value = 0);
    ~WSemaphore();

    bool tryWait();
    void acquire();
    void release();
};

inline WSemaphore::WSemaphore(int init_value)
#ifdef MACOS
    : _sem(init_value)
#endif
{
#ifndef MACOS
    sem_init(&_sem, 0, init_value);
#endif
}

inline WSemaphore::~WSemaphore()
{
#ifndef MACOS
    sem_destroy(&_sem);
#endif
}

inline void WSemaphore::acquire()
{
#ifdef MACOS
    _sem.acquire();
#else
    sem_wait(&_sem);
#endif
}

inline void WSemaphore::release()
{
#ifdef MACOS
    _sem.release();
#else
    sem_post(&_sem);
#endif
}

inline bool WSemaphore::tryWait()
{
    return this->_sem.try_acquire();
}
