#pragma once

#include "WMutex.h"
#include "WMutexLocker.h"
#include <atomic>

template <typename T>
class AtomicSafe {
private:
    T _value;
    mutable WMutex _locker;
public:
    static_assert(!std::is_pointer<T>::value, "template type must not be a pointer type");

    explicit AtomicSafe(const T value) : _value(value), _locker() {}
    ~AtomicSafe() = default;

    T value() const {
        WMutexLocker _(this->_locker);
        return this->_value;
    }

    bool operator==(const AtomicSafe &value) const {
        const T val = value.value();
        WMutexLocker _(this->_locker);
        return this->_value == val;
    }

    bool operator!=(const AtomicSafe &value) const {
        return value != *this;
    }

    AtomicSafe& operator=(const T &value) {
        WMutexLocker _(this->_locker);
        this->_value = value;
        return *this;
    }

    AtomicSafe& operator=(const AtomicSafe &value) {
        const T val = value.get();

        WMutexLocker _(this->_locker);
        this->_value = value._value;
        return *this;
    }

    const T& operator*() {
        WMutexLocker _(this->_locker);
        return this->_value;
    }

    T operator++() {
        WMutexLocker _(this->_locker);
        return ++this->_value;
    }

    T operator--() {
        WMutexLocker _(this->_locker);
        return --this->_value;
    }

    T operator*() const {
        WMutexLocker _(this->_locker);
        return this->_value;
    }

    T operator() (AtomicSafe<T> &value) {
        WMutexLocker _(value._locker);
        return this->value;
    }

    T operator-=(const T &value) {
        WMutexLocker _(this->_locker);
        return this->_value -= value;
    }

    T operator+=(const T &value) {
        WMutexLocker _(this->_locker);
        return this->_value += value;
    }

    bool operator==(const T val) const {
        WMutexLocker _(this->_locker);
        return this->_value == val;
    }

    bool operator!=(const T val) const {
        return (this->operator==(val));
    }
};


