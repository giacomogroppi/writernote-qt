#pragma once

#include "core/WMutex.h"
#include "core/WFlags.h"

template <class T>
class WMutexLocker
{
public:
    enum Flag {
        Lock = 0x1,
        NotLock = 0x2
    };

    explicit WMutexLocker(T &mutex, WFlags<Flag> flags = Flag::Lock);
    WMutexLocker(const WMutexLocker& other) noexcept = delete;
    WMutexLocker(WMutexLocker&& other) noexcept = delete;
    ~WMutexLocker();

    template <class Callable, class ...Args>
    static void atomically(T &mutex, Callable method, Args&& ...args);

    auto operator=(const WMutexLocker& other) -> WMutexLocker = delete;
    auto operator=(WMutexLocker&& other) -> WMutexLocker = delete;

private:
    T &_m;
    const WFlags<Flag> _flags;
    static_assert(!std::is_pointer<T>::value);
};

template <class T>
template <class Callable, class ...Args>
void WMutexLocker<T>::atomically(T& mutex, Callable method, Args&& ...args)
{
    WMutexLocker guard(mutex);
    method(std::forward<Args>(args)...);
}

template <class T>
WMutexLocker<T>::WMutexLocker (T &mutex, WFlags<Flag> flags)
    : _m(mutex)
    , _flags(flags)
{
    if (_flags & Flag::Lock)
        _m.lock();
}

template <class T>
WMutexLocker<T>::~WMutexLocker()
{
    if (_flags & Flag::Lock)
        _m.unlock();
}
