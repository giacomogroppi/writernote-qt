#pragma once

#include "WMutex.h"
#include "WMutexLocker.h"
#include <atomic>

/*
template <typename T>
class AtomicSafe {
private:
    T second;
    mutable WMutex _locker;
public:
    static_assert(!std::is_pointer<T>::value, "template type must not be a pointer type");

    explicit AtomicSafe(const T value) : second(value), _locker() {}
    ~AtomicSafe() = default;

    T value() const {
        WMutexLocker _(this->_locker);
        return this->second;
    }

    bool operator==(const AtomicSafe &value) const {
        const T val = value.value();
        WMutexLocker _(this->_locker);
        return this->second == val;
    }

    bool operator!=(const AtomicSafe &value) const {
        return value != *this;
    }

    AtomicSafe& operator=(const T &value) {
        WMutexLocker _(this->_locker);
        this->second = value;
        return *this;
    }

    AtomicSafe& operator=(const AtomicSafe &value) {
        const T val = value.get();

        WMutexLocker _(this->_locker);
        this->second = value.second;
        return *this;
    }

    const T& operator*() {
        WMutexLocker _(this->_locker);
        return this->second;
    }

    T operator++() {
        WMutexLocker _(this->_locker);
        return ++this->second;
    }

    T operator--() {
        WMutexLocker _(this->_locker);
        return --this->second;
    }

    T operator*() const {
        WMutexLocker _(this->_locker);
        return this->second;
    }

    T operator() (AtomicSafe<T> &value) const {
        WMutexLocker _(value._locker);
        return this->value;
    }

    T operator-=(const T &value) {
        WMutexLocker _(this->_locker);
        return this->second -= value;
    }

    T operator+=(const T &value) {
        WMutexLocker _(this->_locker);
        return this->second += value;
    }

    bool operator==(const T val) const {
        WMutexLocker _(this->_locker);
        return this->second == val;
    }

    bool operator!=(const T val) const {
        return (this->operator==(val));
    }
};
*/
template<class T>
using AtomicSafe = std::atomic<T>;