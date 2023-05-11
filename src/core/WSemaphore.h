#pragma once

#include "utils/WCommonScript.h"

#include <iostream>
#include <semaphore>

class WSemaphore {
private:
    std::binary_semaphore _sem;
public:
    explicit WSemaphore(int init_value = 0);
    ~WSemaphore();

    bool tryWait();
    void acquire();
    void release();
};

inline WSemaphore::WSemaphore(int init_value)
    : _sem(init_value)
{
}

inline WSemaphore::~WSemaphore() = default;

inline void WSemaphore::acquire()
{
    _sem.acquire();
}

inline void WSemaphore::release()
{
    _sem.release();
}

inline bool WSemaphore::tryWait()
{
    return this->_sem.try_acquire();
}
