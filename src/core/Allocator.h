#pragma once

#include "utils/WCommonScript.h"
#include "Scheduler/Scheduler.h"
#include "core/pointer/UniquePtr.h"

/**
 * \tparam T class for the allocator
 * \tparam useCache true if you want to use cache
 * \tparam useMultipleList true if you want to use lock-free algorithm
 * \tparam useRandom true if you want to use randomness instead of thread identifier
 * */
template <class T,
        bool callDestructor,
        bool useCache,
        bool useMultipleList = true,
        bool useRandom = false>
class Allocator
{
private:
    using Mutex = std::mutex;

    template <class T2>
    using List = std::list<T2>;

    template <class T2>
    using Vector = std::vector<T2>;

    Vector<UniquePtr<Mutex>> _lock;
    Vector<List<T*>> _per_cpu_items;

    /**
     * \return identifier for the current thread.
     *  This method take into account if we are using random or scheduler identifier
     * */
    nd auto getIdentifier() const -> int;

    // TODO: use some lock-free algorithm

    const Fn<T*()> _allocateNew;
    const Fn<void(T*)> _dealloc;
public:
    explicit Allocator(Fn<T*()> allocateNew, Fn<void(T*)> dealloc);
    ~Allocator();

    template <typename ...Args>
    auto get(Args&& ...args) -> T*;

    auto put (T*) -> void;
};

template <class T, bool callDestructor, bool useCache, bool useMultipleList, bool useRandom>
auto Allocator<T, callDestructor, useCache, useMultipleList, useRandom>::getIdentifier() const -> int
{
    W_ASSERT(useCache == true);

    if constexpr (useMultipleList) {
        const auto id = (useRandom
                                ? std::rand()
                                : Scheduler::getThreadIdentifier()
                        ) % _per_cpu_items.size();
        return id;
    } else {
        return 0;
    }
}


template <class T, bool callDestructor, bool useCache, bool useMultipleList, bool useRandom>
Allocator<T, callDestructor, useCache, useMultipleList, useRandom>::~Allocator()
{
    if constexpr (!useCache)
        return;

    for (auto &per_cpu: _per_cpu_items)
        for (auto* item: per_cpu)
            _dealloc(item);
}

template <class T, bool callDestructor, bool useCache, bool useMultipleList, bool useRandom>
Allocator<T,
    callDestructor,
    useCache,
    useMultipleList,
    useRandom>::Allocator(
            Fn<T*()> allocateNew,
            Fn<void(T*)> dealloc
    )
    : _allocateNew (std::move(allocateNew))
    , _dealloc (std::move(dealloc))
{
    const unsigned numberOfThreads = (useMultipleList)
            ? Scheduler::numberOfThread()
            : 1;

    for (unsigned i = 0u; i < numberOfThreads; i++) {
        this->_per_cpu_items.push_back({});
        this->_lock.push_back(std::make_unique<Mutex>());
    }
}

template <class T, bool callDestructor, bool useCache, bool useMultipleList, bool useRandom>
template <typename ...Args>
auto Allocator<T, callDestructor, useCache, useMultipleList, useRandom>::get(Args&& ...args) -> T *
{
    if constexpr (!useCache)
        return _allocateNew();

    W_ASSERT(_per_cpu_items.size() > 0);

    const auto removeFromVector = [&] (auto &list) -> T* {
        auto *result = list.front();
        list.pop_front();
        return result;
    };

    const auto allocateForVector = [&] (int numberOfObjects) -> List<T*> {
        List<T*> objects;

        for (int i = 0; i < numberOfObjects; i++)
            objects.push_back(_allocateNew());
        return objects;
    };

    const auto id = this->getIdentifier();

    auto &ref = _per_cpu_items[id];
    std::unique_lock guard (*_lock[id]);

    if (ref.empty())
        ref = std::move(allocateForVector(64));

    auto *object = removeFromVector(ref);

    new (object) T (std::forward<Args>(args)...);
    return object;
}

template <class T, bool callDestructor, bool useCache, bool useMultipleList, bool useRandom>
auto Allocator<T, callDestructor, useCache, useMultipleList, useRandom>::put(T *object) -> void
{
    if constexpr (!useCache)
        _dealloc(object);

    W_ASSERT(object != nullptr);

    if constexpr (callDestructor)
        object->~T();

    const auto id = this->getIdentifier();
    auto &ref = _per_cpu_items[id];

    std::unique_lock guard (*_lock[id]);
    ref.push_back(object);
}