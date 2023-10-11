#pragma once

#include <iostream>
#include "utils/WCommonScript.h"
#include "VersionFileController.h"
#include "Writable.h"

template <class K, class T>
class WPair
{
public:
    explicit WPair() = default;

    template <class K2 = K, class T2 = T>
    WPair(const K2 &key, const T2 &value);

    template <class K2 = K, class T2 = T>
    WPair(K2 &&key, T2 &&value) noexcept;

    template <class K2 = K, class T2 = T>
    WPair(const K2& key, T2 &&value) noexcept;

    template <class K2 = K, class T2 = T>
    WPair(K2 &&key, const T2 &value) noexcept;

    template <class K2 = K, class T2 = T>
    WPair(const WPair<K2, T2> &other) noexcept;

    template <class K2 = K, class T2 = T>
    WPair(WPair<K2, T2> &&other) noexcept;

    ~WPair() = default;

    auto getKey() const -> const K& { return this->first; };
    auto getKey() -> K& { return this->first; };

    auto getValue() const -> const T& { return this->second; };
    auto getValue () -> T& { return this->second; };

    void setKey(const K &key) { this->first = key; };
    void setValue(const T &value) { this->second = value; };

    /**
     * \return value &lt 0 iff it fail
     * */
    template <class K2 = K, class T2 = T>
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WPair<K2, T2>>
    {
        WPair<K2, T2> result;

        if (versionController.getVersionWPair() != 0)
            return {-1, result};

        {
            auto [k, data] = K2::load (versionController, readable);
            if (k < 0)
                return {-1, result};
            result.first = std::move (data);
        }

        {
            auto [k, data] = T2::load(versionController, readable);
            if (k < 0)
                return {-1, result};
            result.second = std::move (data);
        }

        return {0, result};
    }

    template <class K2, class T2>
    static auto write (WritableAbstract &writable, const WPair<K2, T2> &object)
    {
        if (K2::write(writable, object.first) < 0)
            return -1;
        if (T2::write(writable, object.second) < 0)
            return -1;
        return 0;
    }

    template <class K2 = K, class T2 = T>
    auto operator=(WPair<K2, T2> &&other) noexcept -> WPair<K, T> &;

    template <class K2 = K, class T2 = T>
    auto operator=(const WPair<K2, T2> &other) noexcept -> WPair<K, T> &;

    auto operator==(const WPair<K, T> &other) const -> bool
    {
        if (this == &other)
            return true;
        return this->first == other.first &&
               this->second == other.second;
    }

    static constexpr int currentVersion = 0;
    K first;
    T second;
};

template<class K, class T>
template<class K2, class T2>
inline auto WPair<K, T>::operator=(const WPair<K2, T2> &other) noexcept -> WPair<K, T> &
{
    this->first = other.first;
    this->second = other.second;
    return *this;
}

template<class K, class T>
template <class K2, class T2>
inline auto WPair<K, T>::operator=(WPair<K2, T2> &&other) noexcept -> WPair<K, T> &
{
    second = std::move (other.second);
    first = std::move(other.first);
    return *this;
}

template<class K, class T>
template <class K2, class T2>
WPair<K, T>::WPair(WPair<K2, T2> &&other) noexcept
    : first (std::move (other.first))
    , second (std::move(other.second))
{

}

template<class K, class T>
template <class K2, class T2>
inline WPair<K, T>::WPair(const WPair<K2, T2> &other) noexcept
    : first(other.first)
    , second(other.second)
{

}

template<class K, class T>
template <class K2, class T2>
inline WPair<K, T>::WPair(K2 &&key, const T2 &value) noexcept
    : first(std::forward<K2>(key))
    , second(value)
{
}

template<class K, class T>
template <class K2, class T2>
inline WPair<K, T>::WPair(const K2 &key, T2 &&value) noexcept
    : first(key)
    , second(std::forward<T2>(value))
{
}

template<class K, class T>
template <class K2, class T2>
inline WPair<K, T>::WPair(K2 &&key, T2 &&value) noexcept
    : first (std::forward<K2>(key))
    , second(std::forward<T2>(value))
{
}

template<class K, class T>
template <class K2, class T2>
inline WPair<K, T>::WPair(const K2 &key, const T2 &value)
    : first(key)
    , second(value)
{
}
