#pragma once

#include "core/WMutex.h"
#include "core/WMutexLocker.h"

template <class T>
class SharedPtrThreadSafe
{
    mutable int *_count;
    mutable WMutex _lock;
    T* _object;
public:
    SharedPtrThreadSafe();
    ~SharedPtrThreadSafe();

    explicit SharedPtrThreadSafe(T* object);

    SharedPtrThreadSafe(const SharedPtrThreadSafe& other);
    SharedPtrThreadSafe(SharedPtrThreadSafe&& other);

    auto operator->() const -> const T*;
    auto operator*() const -> const T&;

    operator bool() const;
};

template <class T>
SharedPtrThreadSafe<T>::SharedPtrThreadSafe(T *object)
    : _count(new int(0))
    , _object(object)
    , _lock()
{

}

template <class T>
SharedPtrThreadSafe<T>::SharedPtrThreadSafe()
    : _count(new int(0))
    , _object(nullptr)
    , _lock()
{

}

template <class T>
SharedPtrThreadSafe<T>::~SharedPtrThreadSafe()
{
    WMutexLocker _(_lock);

    if (*_count == 1) {
        delete _object;
        delete _count;

        _object = nullptr;
        _count = nullptr;
    } else {
        *_count --;
    }
}