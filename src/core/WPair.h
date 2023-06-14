#pragma once

#include <iostream>

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

    const K& getKey() const {return this->_key;};
    K& getKey() {return this->_key;};

    const T& getValue() const {return this->_value;};
    T& getValue () {return this->_value;};

    void setKey(const K &key) {this->_key = key;};
    void setValue(const T &value) {this->_value = value;};



    bool operator==(const WPair<K, T> &other)
    {
        if (this == &other)
            return true;
        return this->_key == other._key &&
               this->_value == other._value;
    }
};

template<class K, class T, typename T3>
inline WPair<K, T, T3>::WPair(const K &key, const T &value)
    : _key(key)
    , _value(value)
{
}
