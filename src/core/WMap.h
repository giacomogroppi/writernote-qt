#pragma once

#include <unordered_map>

template<class Key, class Value, class Hasher>
class WMap
{
    std::unordered_map<Key, Value, Hasher> _d;
public:
    WMap() = default;
    ~WMap() = default;

    auto get (const Key& key) const -> const Value&;
    auto get (const Key& key) -> Value&;

    auto remove (const Key& key) -> Value;

    auto set (const Key& key, const Value &value) -> void;
    auto set (Key &&key, Value &&value) -> void;
    auto set (const Key& key, Value &&value) -> void;
    auto set (Key &&key, const Value& value) -> void;

    auto isPresent(const Key &key) const noexcept -> bool;
};

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::set(const Key &key, const Value &value) -> void
{
    if (isPresent(key))
        remove(key);
    this->_d.emplace(key, value);
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::set(const Key &key, Value &&value) -> void
{
    if (isPresent(key))
        remove(key);
    this->_d.emplace(key, std::move(value));
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::set(Key &&key, const Value &value) -> void
{
    if (isPresent(key))
        remove(key);
    this->_d.emplace(std::move(key), value);
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::set(Key &&key, Value &&value) -> void
{
    if (isPresent(key))
        remove(key);
    this->_d.emplace(std::move(key), std::move(value));
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::isPresent(const Key &key) const noexcept -> bool
{
    return this->_d.contains(key);
}


template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::remove(const Key &key) -> Value
{
    Value v = this->_d.at(key);
    this->_d.erase(key);
    return v;
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::get(const Key &key) -> Value &
{
    W_ASSERT(_d.contains(key));
    return this->_d.at(key);
}

template<class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::get(const Key &key) const -> const Value &
{
    return this->_d.at(key);
}



