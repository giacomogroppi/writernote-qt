#pragma once

#include "utils/WCommonScript.h"
#include <mutex>

class WMutex
{
private:
    mutable std::mutex _m;
public:
    WMutex();

    WMutex(const WMutex &other) = delete;
    WMutex(WMutex &&other) = delete;

    auto operator=(WMutex&& other) -> WMutex& = delete;
    auto operator=(const WMutex &other) -> WMutex & = delete;

    ~WMutex();
private:
    void lock();
    void unlock();
    bool isLocked() const;

    template<class T>
    friend class WMutexLocker;
    friend class Page;
};

force_inline void WMutex::lock()
{
    _m.lock();
}

force_inline void WMutex::unlock()
{
    _m.unlock();
}

force_inline WMutex::~WMutex()
{
}

force_inline WMutex::WMutex()
    : _m()
{
}

inline bool WMutex::isLocked() const
{
    return _m.try_lock();
}

