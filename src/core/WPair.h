#pragma once

#include <iostream>
#include "utils/WCommonScript.h"
#include "VersionFileController.h"
#include "Writable.h"

template <class K, class T>
class WPair
{
private:
    K _key;
    T _value;
public:
    explicit WPair() = default;
    explicit WPair(const K &key, const T &value);

    explicit WPair(K &&key, T &&value) noexcept;
    explicit WPair(const K& key, T &&value) noexcept;
    explicit WPair(K &&key, const T &value) noexcept;

    WPair(const WPair<K, T> &other) noexcept;
    WPair(WPair<K, T> &&other) noexcept;

    ~WPair() = default;

    auto getKey() const -> const K& { return this->_key; };
    auto getKey() -> K& { return this->_key; };

    auto getValue() const -> const T& { return this->_value; };
    auto getValue () -> T& { return this->_value; };

    void setKey(const K &key) { this->_key = key; };
    void setValue(const T &value) { this->_value = value; };

    /**
     * \return value &lt 0 iff it fail
     * */
    template <class K2 = K, class T2 = T>
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, WPair<K2, T2>>
    {
        WPair<K2, T2> result;

        if (versionController.getVersionWPair() != 0)
            return {-1, result};

        {
            auto [k, data] = K2::load (versionController, readable);
            if (k < 0)
                return {-1, result};
            result._key = std::move (data);
        }

        {
            auto [k, data] = T2::load(versionController, readable);
            if (k < 0)
                return {-1, result};
            result._value = std::move (data);
        }

        return {0, result};
    }

    template <class K2, class T2>
    static auto write (WritableAbstract &writable, const WPair<K2, T2> &object)
    {
        if (K2::write(writable, object._key) < 0)
            return -1;
        if (T2::write(writable, object._value) < 0)
            return -1;
        return 0;
    }

    auto operator=(WPair<K, T> &&other) noexcept -> WPair<K, T> &;
    auto operator=(const WPair<K, T> &other) noexcept -> WPair<K, T> &;

    auto operator==(const WPair<K, T> &other) -> bool
    {
        if (this == &other)
            return true;
        return this->_key == other._key &&
               this->_value == other._value;
    }

    static constexpr int currentVersion = 0;
};

template<class K, class T>
inline auto WPair<K, T>::operator=(WPair<K, T> &&other) noexcept -> WPair<K, T> &
{
    _value = std::move (other._value);
    _key = std::move(other._key);
    return *this;
}

template<class K, class T>
WPair<K, T>::WPair(WPair<K, T> &&other) noexcept
    : _key (std::move (other._key))
    , _value (std::move(other._value))
{

}

template<class K, class T>
inline WPair<K, T>::WPair(const WPair<K, T> &other) noexcept
    : _key(other._key)
    , _value(other._value)
{

}

template<class K, class T>
inline WPair<K, T>::WPair(K &&key, const T &value) noexcept
    : _key(std::move(key))
    , _value(value)
{

}

template<class K, class T>
inline WPair<K, T>::WPair(const K &key, T &&value) noexcept
    : _key(key)
    , _value(std::move(value))
{
}

template<class K, class T>
inline WPair<K, T>::WPair(K &&key, T &&value) noexcept
    : _key (std::move(key))
    , _value(std::move(value))
{
}

template<class K, class T>
inline WPair<K, T>::WPair(const K &key, const T &value)
    : _key(key)
    , _value(value)
{
}
