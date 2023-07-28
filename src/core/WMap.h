#pragma once

#include <unordered_map>
#include "VersionFileController.h"
#include "core/WPair.h"

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

    static auto load (const VersionFileController &version, ReadableAbstract &readable) -> WPair<int, WMap<Key, Value, Hasher>>;
    static auto write (WritableAbstract &writable, const WMap &object) -> int;

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

template <class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::load (
            const VersionFileController &version,
            ReadableAbstract &readable
        ) -> WPair<int, WMap<Key, Value, Hasher>>
{
    if (version.getVersionWMap() > 0)
        return {-1, {}};

    int size;
    WMap<Key, Value, Hasher> result;

    if (readable.read(size) < 0)
        return {-1, {}};

    for (int i = 0; i < size; i++) {
        auto [res1, key] = Key::load (version, readable);
        if (res1 < 0) return {-1, {}};

        auto [res2, value] = Value::load (version, readable);
        if (res2 < 0) return {-1, {}};

        result.set(std::move(key), std::move(value));
    }

    return {0, result};
}

template <class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::write (
        WritableAbstract &writable,
        const WMap &object
    ) -> int
{
    const int size = object._d.size();
    if (writable.write(size) < 0)
        return -1;

    for (const auto &[key, item]: std::as_const(object._d)) {
        if (Key::write (writable, key) < 0)
            return -1;
        if (Value::write (writable, item) < 0)
            return -1;
    }

    return 0;
}
