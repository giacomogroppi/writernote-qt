#pragma once

#define SEM_UNIX

#include "utils/WCommonScript.h"
#ifdef SEM_UNIX
#include <semaphore>
#else
# include "semaphore.h"
#endif

class WSemaphore {
private:
#ifdef SEM_UNIX
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
#ifdef SEM_UNIX
    : _sem(init_value)
#endif
{
#ifndef SEM_UNIX
    sem_init(&_sem, 0, init_value);
#endif
}

inline WSemaphore::~WSemaphore()
{
#ifndef SEM_UNIX
    sem_destroy(&_sem);
#endif
}

inline void WSemaphore::acquire()
{
#ifdef SEM_UNIX
    _sem.acquire();
#else
    sem_wait(&_sem);
#endif
}

inline void WSemaphore::release()
{
#ifdef SEM_UNIX
    _sem.release();
#else
    sem_post(&_sem);
#endif
}

inline bool WSemaphore::tryWait()
{
#ifdef SEM_UNIX
    return this->_sem.try_acquire();
#else
    W_ASSERT(0);
    return false;
#endif
}
