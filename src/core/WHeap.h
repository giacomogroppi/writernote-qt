#pragma once

#include <algorithm>
#include <vector>

/**
 * \tparam GetValue Class with one static method called getValue that return a value for comparison
 * \tparam min Set this value to false if you want to create a heap in descending order
 * \tparam T the value to store
 * */
template <class T, class GetValue, bool min = true>
class WHeap
{
private:
    std::vector<T> _d;

    /**
     * It will create a heap
     * \return True if the data structure has been modified
     * */
    auto createHeap() -> bool;
public:
    WHeap() = default;
    ~WHeap() = default;

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
};


