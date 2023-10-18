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

    static auto load (const VersionFileController &version, ReadableAbstract &readable) -> WPair<Error, WMap<Key, Value, Hasher>>;
    static auto write (WritableAbstract &writable, const WMap &object) -> Error;

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
        ) -> WPair<Error, WMap<Key, Value, Hasher>>
{
    if (version.getVersionWMap() > 0)
        return {Error::makeErrVersion(), {}};

    int size;
    WMap<Key, Value, Hasher> result;

    if (auto err = readable.read(size))
        return {err, {}};

    for (int i = 0; i < size; i++) {
        auto [res1, key] = Key::load (version, readable);
        if (res1) 
            return {res1, {}};

        auto [res2, value] = Value::load (version, readable);
        if (res2) 
            return {res2, {}};

        result.set(std::move(key), std::move(value));
    }

    return {Error::makeOk(), result};
}

template <class Key, class Value, class Hasher>
inline auto WMap<Key, Value, Hasher>::write (
        WritableAbstract &writable,
        const WMap &object
    ) -> Error
{
    const int size = object._d.size();
    if (auto err = writable.write(size))
        return err;

    for (const auto &[key, item]: std::as_const(object._d)) {
        if (auto err = Key::write (writable, key))
            return err;
        if (auto err = Value::write (writable, item))
            return err;
    }

    return Error::makeOk();
}
