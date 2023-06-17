#pragma once

#include <iostream>
#include "utils/WCommonScript.h"
#include "VersionFileController.h"
#include "Writable.h"

template <class K, class T, typename = std::enable_if_t<!std::is_pointer_v<K> && !std::is_pointer_v<T>>>
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
    template <class K2, class T2, class Readable>
    static auto load (const VersionFileController &versionController, Readable &readable, WPair<K2, T2> &result) -> int
    {
        if (versionController.getVersionWPair() != 0)
            return -1;
        if (K2::load(versionController, readable, result._key) < 0)
            return -1;

        if (T2::load(versionController, readable, result._value) < 0)
            return -1;
        return 0;
    }

    template <class K2, class T2, class Writable>
        requires (std::is_base_of_v<WritableAbstract, Writable>)
    static auto save (Writable &writable, const WPair<K2, T2> &object)
    {
        if (K2::save(writable, object._key) < 0)
            return -1;
        if (T2::save(writable, object._value) < 0)
            return -1;
        return 0;
    }

    auto operator==(const WPair<K, T> &other) -> bool
    {
        if (this == &other)
            return true;
        return this->_key == other._key &&
               this->_value == other._value;
    }

    static constexpr int currentVersion = 0;
};

template<class K, class T, typename T3>
inline WPair<K, T, T3>::WPair(const K &key, const T &value)
    : _key(key)
    , _value(value)
{
}
