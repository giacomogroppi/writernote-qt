#pragma once

#include "core/WMutex.h"
#include "core/WMutexLocker.h"
#include "core/WRecursiveLock.h"
#include <concepts>
#include "core/AtomicSafe.h"

//
// #define SHARED_PTR_THREAD_SAFE_USE_MUTEX
#ifdef SHARED_PTR_THREAD_SAFE_USE_MUTEX
template <class T>
class SharedPtrThreadSafe
{
    mutable int *_count;
    mutable WRecursiveLock *_lock;
    T* _object;

    void rep() const noexcept
    {
        if (_object) {
            W_ASSERT(_lock != nullptr and _count != nullptr);
        } else {
            W_ASSERT(_lock == nullptr and _count == nullptr);
        }
    }

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

    void doAndUnref (Fn<void(T&)> method);

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

    rep();
}

template <class T>
void SharedPtrThreadSafe<T>::release() noexcept
{
    *this = SharedPtrThreadSafe<T>(nullptr);
    rep();
}

template <class T>
void SharedPtrThreadSafe<T>::doAndUnref(Fn<void(T&)> method)
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

    rep();
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

    rep();
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

    rep();

    return *this;
}

template <class T>
auto SharedPtrThreadSafe<T>::operator=(SharedPtrThreadSafe&& other) -> SharedPtrThreadSafe&
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

    rep();

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
    rep();
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
    rep();
}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe(T *object)
        : _count(new int(1))
        , _lock(new WRecursiveLock)
        , _object(object)
{
    if (object == nullptr)
        destroy();
    rep();
}

template <class T>
inline SharedPtrThreadSafe<T>::SharedPtrThreadSafe()
        : _count(nullptr)
        , _object(nullptr)
        , _lock(nullptr)
{
    rep();
}

template <class T>
inline SharedPtrThreadSafe<T>::~SharedPtrThreadSafe()
{
    destroy();
    rep();
}

template <class T>
inline auto SharedPtrThreadSafe<T>::isUnique() const -> bool
{
    if (_count == nullptr)
        return true;

    WMutexLocker guard(*_lock);
    if (*_count == 1) {
        rep();
        return true;
    }

    rep();

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
    rep();
    return *_count;
}
#else
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

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    SharedPtrThreadSafe(const SharedPtrThreadSafe<Z>& object);

    template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
    SharedPtrThreadSafe(SharedPtrThreadSafe<Z>&& object);

    SharedPtrThreadSafe(const SharedPtrThreadSafe& other);
    SharedPtrThreadSafe(SharedPtrThreadSafe&& other);

    auto isUnique() const -> bool;
    auto get() const -> T*;

    auto numberOfRef() const -> int;

    template <class Func>
    auto atomically(T func) -> void;

    template <class ...Args>
    static auto make (Args&& ...args)
    {
        return SharedPtrThreadSafe<T>(new T(std::forward<Args>(args)...));
    }

    auto operator->() const -> const T*;
    auto operator->() -> T*;
    auto operator*() const -> const T&;
    auto operator*() -> T&;

    explicit operator bool() const;

    void doAndUnref (Fn<void(T&)> method);

    void release() noexcept;

    auto operator=(const SharedPtrThreadSafe& other) -> SharedPtrThreadSafe&;
    auto operator=(SharedPtrThreadSafe&& other) -> SharedPtrThreadSafe&;

    auto operator==(const SharedPtrThreadSafe& other) const -> bool;

    template <class Z>
    friend class SharedPtrThreadSafe;
};

template<class T>
auto SharedPtrThreadSafe<T>::operator==(const SharedPtrThreadSafe& other) const -> bool
{
    return other._object == _object;
}

template <class T>
void SharedPtrThreadSafe<T>::release() noexcept
{
    *this = SharedPtrThreadSafe<T>(nullptr);
}

template <class T>
void SharedPtrThreadSafe<T>::doAndUnref(Fn<void(T&)> method)
{
    const auto numberOfRef = --(*_counter);

    method(*_object);

    if (numberOfRef == 0) {
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
        auto valueWritten = --(*_counter);
        if (valueWritten == 0) {

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

    *other._counter += 1;

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

    if (*_counter != 1)
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

template <class T>
template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
SharedPtrThreadSafe<T>::SharedPtrThreadSafe(const SharedPtrThreadSafe<Z>& object)
    : _counter(object._counter)
    , _object(object._object)
{
    if (_counter) {
        *_counter += 1;
    }
}

template <class T>
template <class Z> requires (not std::is_pointer<Z>::value and std::is_convertible<Z*, T*>::value)
SharedPtrThreadSafe<T>::SharedPtrThreadSafe(SharedPtrThreadSafe<Z>&& object)
    : _counter(object._counter)
    , _object(object._object)
{
    object._counter = nullptr;
    object._object = nullptr;
}

#endif // SHARED_PTR_THREAD_SAFE_USE_MUTEX
