#pragma once

#include "WMutex.h"
#include "WMutexLocker.h"

template <typename T>
class AtomicSafe {
private:
    T _value;
    mutable WMutex _locker;
public:
    explicit AtomicSafe(const T value) : _value(value), _locker() {}
    ~AtomicSafe() = default;

    T value() const {
        WMutexLocker _(this->_locker);
        return this->_value;
    }

    bool operator==(const AtomicSafe &value) const {
        WMutexLocker _(this->_locker);
        return this->_value == value._value;
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
};


