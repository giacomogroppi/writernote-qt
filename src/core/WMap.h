#pragma once

#include <unordered_map>

template<class Key, class Value>
class WMap
{
    std::unordered_map<Key, Value> _d;
public:
    WMap() = default;
    ~WMap() = default;

    auto get (const Key& key) const -> const Value&;
    auto get (const Key& key) -> Value&;

    auto remove (const Key& key) -> Value;

    auto set (const Key& key, const Value &value);
    auto set (Key &&key, Value &&value);
    auto set (const Key& key, Value &&value);
    auto set (Key &&key, const Value& value);
};

template<class Key, class Value>
inline auto WMap<Key, Value>::remove(const Key &key) -> Value
{
    Value v = this->_d.at(key);
    this->_d.erase(key);
    return v;
}

template<class Key, class Value>
inline auto WMap<Key, Value>::get(const Key &key) -> Value &
{
    return this->_d.at(key);
}

template<class Key, class Value>
inline auto WMap<Key, Value>::get(const Key &key) const -> const Value &
{
    return this->_d.at(key);
}



