#pragma once

#include "core/WMutex.h"

template <class T>
class WMutexLocker
{
private:
    T &_m;
public:
    WMutexLocker(T &mutex);
    ~WMutexLocker();
};

template <class T>
inline WMutexLocker<T>::WMutexLocker (T &mutex)
    : _m(mutex)
{
    _m.lock();
}

template <class T>
inline WMutexLocker<T>::~WMutexLocker()
{
    _m.unlock();
}
