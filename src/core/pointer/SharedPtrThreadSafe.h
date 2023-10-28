#pragma once

#include "core/WMutex.h"
#include "core/WMutexLocker.h"
#include "core/WRecursiveLock.h"
#include <concepts>
#include "core/AtomicSafe.h"

template <class T>
class SharedPtrThreadSafe
{
    mutable AtomicSafe<int> *_counter;
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
    W_ASSERT(_counter);
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
    method(*_object);
    *_counter -= 1;

    if (*_counter == 0) {
        delete _object;
        delete _counter;
    }

    _counter = nullptr;
    _object = nullptr;
}

template<class T>
inline auto SharedPtrThreadSafe<T>::operator->() const -> const T*
{
    return _object;
}

template <class T>
inline void SharedPtrThreadSafe<T>::destroy()
{
    if (_counter) {
        *_counter -= 1;
        if (*_counter == 0) {

            delete _object;
            delete _counter;

            _object = nullptr;
            _counter = nullptr;
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

    if (!other._counter)
        return *this;

    *other._counter ++;

    _counter = other._count;
    _object = other._object;

    return *this;
}

template <class T>
auto SharedPtrThreadSafe<T>::operator=(SharedPtrThreadSafe&& other) -> SharedPtrThreadSafe&
{
    if (this == &other)
        return *this;

    destroy();

    _counter = other._counter;
    _object = other._object;

    other._object = nullptr;
    other._counter = nullptr;

    return *this;
}

template<class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(SharedPtrThreadSafe&& other)
    : _counter(other._counter)
    , _object(other._object)
{
    other._counter = nullptr;
    other._object = nullptr;
}

template<class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(const SharedPtrThreadSafe &other)
    : _counter(other._counter)
    , _object(other._object)
{
    if (_object) {
        *_counter += 1;
    }
}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(T *object)
    : _counter(new AtomicSafe<int>(1))
    , _object(object)
{

}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe()
    : _counter(nullptr)
    , _object(nullptr)
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
    if (_counter == nullptr)
        return false;

    WDebug(true, *_counter);
    if (*_counter == 1) {
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
    if (!_counter)
        return 1;

    return *_counter;
}
