#pragma once

#include "core/WMutex.h"

template <class T>
class WMutexLocker
{
private:
    T &_m;
public:
    WMutexLocker(T &mutex) : _m(mutex) { _m.lock(); };
    ~WMutexLocker() { _m.unlock(); }
};

