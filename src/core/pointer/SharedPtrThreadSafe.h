#pragma once

#include "core/WMutex.h"
#include "core/WMutexLocker.h"
#include "core/WRecursiveLock.h"
#include <concepts>

template <class T>
class SharedPtrThreadSafe
{
    mutable int *_count;
    mutable WRecursiveLock *_lock;
    T* _object;

    void destroy();
public:
    SharedPtrThreadSafe();
    ~SharedPtrThreadSafe();

    explicit SharedPtrThreadSafe(T* object);

    SharedPtrThreadSafe(const SharedPtrThreadSafe& other);
    SharedPtrThreadSafe(SharedPtrThreadSafe&& other);

    auto isUnique() const -> bool;
    auto get() const -> T*;

    auto numberOfRef() const -> int;

    template <class Func>
    auto atomically(T func) -> void;

    auto operator->() const -> const T*;
    auto operator->() -> T*;
    auto operator*() const -> const T&;
    auto operator*() -> T&;

    explicit operator bool() const;

    void doAndUnref (auto method);

    void release() noexcept;

    auto operator=(const SharedPtrThreadSafe& other) -> SharedPtrThreadSafe&;
    auto operator=(SharedPtrThreadSafe&& other) -> SharedPtrThreadSafe&;

    auto operator==(const SharedPtrThreadSafe& other) const -> bool;
};

template<class T>
auto SharedPtrThreadSafe<T>::operator==(const SharedPtrThreadSafe& other) const -> bool
{
    return other._object == _object;
}

template <class T>
template <class Func>
auto SharedPtrThreadSafe<T>::atomically(T func) -> void
{
    W_ASSERT(_lock);
    WMutexLocker guard(*_lock);
    func();
}

template <class T>
void SharedPtrThreadSafe<T>::release() noexcept
{
    *this = SharedPtrThreadSafe<T>(nullptr);
}

template <class T>
void SharedPtrThreadSafe<T>::doAndUnref(auto method)
{
    W_ASSERT(_count);
    _lock->lock();
    method(*_object);
    *_count -= 1;

    if (*_count == 0) {
        _lock->unlock();
        delete _object;
        delete _count;
        delete _lock;
    } else {
        _lock->unlock();
    }

    _lock = nullptr;
    _object = nullptr;
    _count = nullptr;
}

template<class T>
inline auto SharedPtrThreadSafe<T>::operator->() const -> const T*
{
    return _object;
}

template <class T>
inline void SharedPtrThreadSafe<T>::destroy()
{
    if (_lock) {
        _lock->lock();

        if (*_count == 1) {
            _lock->unlock();

            delete _object;
            delete _count;
            delete _lock;

            _object = nullptr;
            _count = nullptr;
            _lock = nullptr;
        } else {
            *_count -= 1;
            _lock->unlock();
        }
    }
}

template<class T>
inline auto SharedPtrThreadSafe<T>::operator->() -> T*
{
    return _object;
}

template<class T>
inline auto SharedPtrThreadSafe<T>::operator*() const -> const T&
{
    return *_object;
}

template<class T>
inline auto SharedPtrThreadSafe<T>::operator*() -> T&
{
    return *_object;
}

template <class T>
inline SharedPtrThreadSafe<T>::operator bool() const
{
    return _object != nullptr;
}

template <class T>
inline auto SharedPtrThreadSafe<T>::operator=(const SharedPtrThreadSafe& other) -> SharedPtrThreadSafe&
{
    if (this == &other)
        return *this;

    destroy();

    WMutexLocker _locker (other._lock);
    _lock = other._lock;
    _count = other._count;
    _object = other._object;

    if (_object != nullptr)
        *_count += 1;

    return *this;
}

template <class T>
inline auto SharedPtrThreadSafe<T>::operator=(SharedPtrThreadSafe&& other) -> SharedPtrThreadSafe&
{
    if (this == &other)
        return *this;

    destroy();

    _count = other._count;
    _lock = other._lock;
    _object = other._object;

    other._object = nullptr;
    other._lock = nullptr;
    other._count = nullptr;

    return *this;
}

template<class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(SharedPtrThreadSafe&& other)
    : _count(other._count)
    , _lock(other._lock)
    , _object(other._object)
{
    other._count = nullptr;
    other._object = nullptr;
    other._lock = nullptr;
}

template<class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(const SharedPtrThreadSafe &other)
    : _count(other._count)
    , _lock(other._lock)
    , _object(other._object)
{
    WMutexLocker _(*_lock);
    if (_object) {
        *_count += 1;
    }
}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(T *object)
    : _count(new int(1))
    , _lock(new WRecursiveLock)
    , _object(object)
{

}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe()
    : _count(nullptr)
    , _object(nullptr)
    , _lock(nullptr)
{

}

template <class T>
inline SharedPtrThreadSafe<T>::~SharedPtrThreadSafe()
{
    destroy();
}

template <class T>
inline auto SharedPtrThreadSafe<T>::isUnique() const -> bool
{
    if (_count == nullptr)
        return false;

    WMutexLocker guard(*_lock);
    if (*_count == 1) {
        return true;
    }

    return false;
}

template <class T>
inline auto SharedPtrThreadSafe<T>::get() const -> T *
{
    return _object;
}

template<class T>
auto SharedPtrThreadSafe<T>::numberOfRef() const -> int
{
    if (!_lock)
        return 1;

    WMutexLocker guard (*_lock);
    return *_count;

}
