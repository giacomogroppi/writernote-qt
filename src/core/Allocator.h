#pragma once

#include "utils/WCommonScript.h"

template <class T, bool useCache>
class Allocator
{
private:
    std::mutex _lock;
    std::list<T*> _items;

    // TODO: use some lock-free algorithm

    const Fn<T*()> _allocateNew;
public:
    explicit Allocator(Fn<T*()> allocateNew);
    ~Allocator();

    auto get() -> T*;
    auto put (T*) -> void;
};

template <class T, bool useCache>
Allocator<T, useCache>::~Allocator()
{
    if constexpr (useCache)
        for (auto* ref: _items)
            delete ref;
}

template <class T, bool useCache>
Allocator<T, useCache>::Allocator(Fn<T*()> allocateNew)
    : _allocateNew(std::move(allocateNew))
{

}

template <class T, bool useCache>
auto Allocator<T, useCache>::get() -> T *
{
    if constexpr (useCache) {
        std::unique_lock guard (_lock);
        if (_items.empty())
            return _allocateNew();

        auto *result = _items.front();
        _items.pop_front();
        return result;
    } else {
        return new T();
    }
}

template <class T, bool useCache>
auto Allocator<T, useCache>::put(T *object) -> void
{
    if constexpr (useCache) {
        W_ASSERT(object != nullptr);

        std::unique_lock guard (_lock);

        _items.push_front(object);
    } else {
        delete object;
    }
}