#pragma once

#include <algorithm>
#include <vector>
#include "core/WVector.h"

/**
 * \tparam min Set this value to false if you want to create a heap in descending order
 * \tparam T the value to store
 * */
template <class T, bool min = true>
class WHeap
{
private:
    WVector<T> _d;

    /**
     * \return true iff v1 >= v2
     * */
    std::function<bool(const T& v1, const T& v2)> _cmpOriginal;

    /**
     * return true iff v1 should be before v2
     */
    std::function<bool(const T& v1, const T& v2)> _cmp;

    /**
     * It will create a heap
     * \return True if the data structure has been modified
     * */
    auto createHeap() -> void;
public:
    /**
     * \param cmp Function that return true iff v1 >= v2
     * */
    WHeap(std::function<bool(const T& v1, const T& v2)> cmp) noexcept;

    template <class T2 = T>
        requires (std::is_pointer_v<T2>)
    WHeap() noexcept;

    template <class T2 = T>
            requires (!std::is_pointer_v<T2>)
    WHeap() noexcept;

    ~WHeap() = default;

    WHeap(const WHeap &other) noexcept = default;
    WHeap(WHeap &&other) noexcept = default;

    /**
     * This method remove the element that is return
     * \return The smallest element in the datastruct if min is true, otherwise it's return the biggest element
     * in the data structure
     * */
    auto takeFirst() -> T;

    /**
     * This method does not remove the element that is return
     * \return The smallest element in the datastruct if min is true, otherwise it's return the biggest element
     * in the data structure
     * */
    auto getFirst() -> const T&;

    auto add (const T& data) -> void;
    auto add (T&& data) -> void;

    /**
     * \param value The value to remove
     * \return True iff we have remove value from the heap
     * */
    bool removeIfPresent(const T &value) noexcept;
};

template <class T, bool min>
inline auto WHeap<T, min>::removeIfPresent(const T &value) noexcept -> bool
{
    return _d.removeOrder<min>(value, _cmpOriginal);
}

template<class T, bool min>
auto WHeap<T, min>::getFirst() -> const T &
{
    return _d.first();
}

template<class T, bool min>
auto WHeap<T, min>::takeFirst() -> T
{
    T returnValue = this->_d.takeFirst();
    createHeap();

    return returnValue;
}

template <class T, bool min>
auto WHeap<T, min>::add(T &&data) -> void
{
    _d.append(std::move(data));
    createHeap();
}

template<class T, bool min>
auto WHeap<T, min>::add(const T &data) -> void
{
    _d.append(data);
    createHeap();
}

template <class T, bool min>
auto WHeap<T, min>::createHeap() -> void
{
    std::make_heap(_d.begin(), _d.end(), _cmp);
}


template <class T, bool min>
template <class T2>
    requires (std::is_pointer_v<T2>)
WHeap<T, min>::WHeap() noexcept
    : _d()
    , _cmpOriginal ([](const T& v1, const T& v2) {
        return *(v1) <= *(v2);
    })
    , _cmp([](const T& v1, const T& v2) -> bool {
        if constexpr (min) {
            return *(v1) < *(v2);
        }
        return *(v1) >= *(v2);
    })
{

}

template <class T, bool min>
template <class T2>
    requires (!std::is_pointer_v<T2>)
WHeap<T, min>::WHeap() noexcept
    : _d()
    , _cmpOriginal ([](const T& v1, const T& v2) {
        return v1 >= v2;
    })
    , _cmp ([](const T& v1, const T& v2) -> bool{
        if constexpr (min)
            return v1 < v2;
        return v1 >= v2;
    })
{

}

template <class T, bool min>
WHeap<T, min>::WHeap(std::function<bool(const T &, const T &)> cmp) noexcept
    : _d()
    , _cmpOriginal(std::move(cmp))
    , _cmp ([this](const T& v1, const T& v2) {
        if constexpr (min) {
            return !this->_cmpOriginal (v1, v2);
        }
        return this->_cmpOriginal (v1, v2);
    })
{

}