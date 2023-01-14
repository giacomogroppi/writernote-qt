#pragma once

#include <semaphore>

class WSemaphore {
private:
    std::counting_semaphore<0> _sem;
public:
    WSemaphore(int init_value);
    ~WSemaphore() = default;

    void acquire();
    void release();
};

inline WSemaphore::WSemaphore(int init_value):
    _sem(init_value)
{}

inline void WSemaphore::acquire()
{
    _sem.acquire();
}

inline void WSemaphore::release()
{
    _sem.release();
}
