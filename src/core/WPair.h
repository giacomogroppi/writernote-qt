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
    explicit WPair(const K &key, const T &value);

    WPair(K &&key, T &&value) noexcept;
    WPair(const K& key, T &&value) noexcept;
    WPair(K &&key, const T &value) noexcept;

    WPair(const WPair<K, T> &other) noexcept;
    WPair(WPair<K, T> &&other) noexcept;

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

    auto operator=(WPair<K, T> &&other) noexcept -> WPair<K, T> &;
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
inline auto WPair<K, T>::operator=(WPair<K, T> &&other) noexcept -> WPair<K, T> &
{
    second = std::move (other.second);
    first = std::move(other.first);
    return *this;
}

template<class K, class T>
WPair<K, T>::WPair(WPair<K, T> &&other) noexcept
    : first (std::move (other.first))
    , second (std::move(other.second))
{

}

template<class K, class T>
inline WPair<K, T>::WPair(const WPair<K, T> &other) noexcept
    : first(other.first)
    , second(other.second)
{

}

template<class K, class T>
inline WPair<K, T>::WPair(K &&key, const T &value) noexcept
    : first(std::move(key))
    , second(value)
{

}

template<class K, class T>
inline WPair<K, T>::WPair(const K &key, T &&value) noexcept
    : first(key)
    , second(std::move(value))
{
}

template<class K, class T>
inline WPair<K, T>::WPair(K &&key, T &&value) noexcept
    : first (std::move(key))
    , second(std::move(value))
{
}

template<class K, class T>
inline WPair<K, T>::WPair(const K &key, const T &value)
    : first(key)
    , second(value)
{
}
