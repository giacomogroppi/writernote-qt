#pragma once

#include <algorithm>
#include <vector>
#include "core/WVector.h"

/**
 * \tparam lowToHigh true if in descending order
 *              false for ascending order
 * \tparam T the value to store
 * */
template <class T, bool lowToHigh = true>
class WHeap
{
private:
    static constexpr auto debug = false;

    WVector<T> _d;

    /**
     * \return true iff v1 > v2
     * */
    Fn<bool(const T& v1, const T& v2)> _cmpOriginal;

    /**
     * \return true iff the public invariant is correct
     * */
    nd auto rep () const -> bool;

    /**
     * It will create a heap
     * */
    auto createHeap() noexcept -> void;
public:
    /**
     * \param cmp Function that return true iff v1 > v2
     * */
    WHeap(Fn<bool(const T& v1, const T& v2)> cmp) noexcept;

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
     * \return The smallest element in the datastruct if lowToHigh is true, otherwise it's return the biggest element
     * in the data structure
     * */
    auto takeFirst() noexcept -> T;

    /**
     * This method does not remove the element that is return
     * \return The smallest element in the datastruct if lowToHigh is true, otherwise it's return the biggest element
     * in the data structure
     * */
    auto getFirst() noexcept -> const T&;

    auto size() const -> int;

    auto add (const T& data) -> void;
    auto add (T&& data) -> void;

    /**
     * \param value The value to remove
     * \return True iff we have remove value from the heap
     * */
    auto removeIfPresent(const T &value) noexcept -> bool;

    /**
     * \brief This method remove the first occurrence of the object
     * \param value The object to remove from the data structure
     * \return True if an item has been removed
     */
    auto removeIfPresentSingle(const T& value) noexcept -> bool;

    auto isEmpty() const noexcept -> bool;
};

template <class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::isEmpty() const noexcept -> bool
{
    return this->_d.isEmpty();
}

template <class T, bool lowToHigh>
inline auto WHeap<T, lowToHigh>::removeIfPresent(const T &value) noexcept -> bool
{
    bool shouldCreateHeap = false;

    if (_d.isEmpty())
        return 0;

    if (value == _d.first())
        shouldCreateHeap = true;

    const auto result = _d.removeIf([&value](const T& tmp) {
        return tmp == value;
    });

    if (shouldCreateHeap)
        createHeap();

    W_ASSERT(rep());

    return result;
}

template <class T, bool lowToHigh>
inline auto WHeap<T, lowToHigh>::removeIfPresentSingle(const T &value) noexcept -> bool
{
    bool shouldMakeHeap = false;

    if (value == _d.first())
        shouldMakeHeap = true;

    const auto result = _d.removeSingle(value);

    if (shouldMakeHeap)
        createHeap();

    W_ASSERT(rep());

    return result;
}

template<class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::getFirst() noexcept -> const T &
{
    return _d.first();
}

template<class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::takeFirst() noexcept -> T
{
    T returnValue = std::forward<T>(this->_d.takeFirst());
    createHeap();

    W_ASSERT(rep());

    return returnValue;
}

template <class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::add(T &&data) -> void
{
    _d.append(std::move(data));
    createHeap();
    W_ASSERT(rep());
}

template<class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::add(const T &data) -> void
{
    _d.append(data);
    createHeap();
    W_ASSERT(rep());
}

template <class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::createHeap() noexcept -> void
{
    const auto cmp = [this] (const T& v1, const T& v2) {
        return !_cmpOriginal(v1, v2);
    };

    if constexpr (lowToHigh)
        std::make_heap(_d.begin(), _d.end(), _cmpOriginal);
    else
        std::make_heap(_d.begin(), _d.end(), cmp);

    W_ASSERT(rep());
}

template <class T, bool lowToHigh>
template <class T2>
    requires (std::is_pointer_v<T2>)
inline WHeap<T, lowToHigh>::WHeap() noexcept
    : _d()
    , _cmpOriginal ([](const T& v1, const T& v2) {
        return *(v1) > *(v2);
    })
{
    W_ASSERT(rep());
}

template <class T, bool lowToHigh>
template <class T2>
    requires (!std::is_pointer_v<T2>)
WHeap<T, lowToHigh>::WHeap() noexcept
    : _d()
    , _cmpOriginal ([](const T& v1, const T& v2) {
        return v1 > v2;
    })
{
    W_ASSERT(rep());
}

template <class T, bool lowToHigh>
WHeap<T, lowToHigh>::WHeap(Fn<bool(const T &, const T &)> cmp) noexcept
    : _d()
    , _cmpOriginal(std::move(cmp))
{
    W_ASSERT(rep());
}

template <class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::size() const -> int
{
    return this->_d.size();
}

template <class T, bool lowToHigh>
auto WHeap<T, lowToHigh>::rep() const -> bool
{
    for (int i = 1; i < _d.size(); i++) {
        const auto first = _d.first();
        const auto current = _d.at(i);
        if constexpr (lowToHigh) {
            if (_cmpOriginal(first, current)) {
                WDebug(debug, first << "is lower than" << current << "... Array:" << _d);
                return false;
            }
        } else {
            if (!_cmpOriginal(first, current)) {
                WDebug(debug, _d);
                return false;
            }
        }
    }

    return true;
}
