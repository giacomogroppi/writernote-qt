#pragma once

#include "core/WMutex.h"

class WMutexLocker
{
private:
    WMutex &_m;
public:
    WMutexLocker(WMutex &mutex) : _m(mutex) { _m.lock(); };
    ~WMutexLocker() { _m.unlock(); }
};

