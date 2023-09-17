#pragma once

#include "utils/WCommonScript.h"
#include <algorithm>

namespace WAbstractList {
    template<class T>
    auto isSorted(const T& list) -> bool
    {
        auto b1 = list.begin();
        auto b2 = b1 + 1;
        const auto end = list.end();

        for (; b2 != end; b1++, b2++) {
            if (b1 > b2)
                return false;
        }

        return true;
    };

    template <class T>
    auto sort (T begin, T end) -> void
    {
        std::sort(begin, end);
    }

    template <class T, class F>
    auto sort (T begin, T end, Fn<bool(const F& v1, const F& v2)> cmp) -> void
    {
        std::sort(begin, end, cmp);
    }

    /**
     * \tparam Iterator The iterator
     * \param begin The begin iterator
     * \param end The end iterator
     * \return end iff object is not between [begin, end)
     * \param cmp needs to return
     *              0 in case v1 == v2
     *              1 in case v1 > v2
     *              -1 in case v1 < v2
     * */
     /*
    template <class Iterator, class T, bool descending = false>
    auto binary_search (
            Iterator begin,
            Iterator end,
            const T& object,
            Fn<int(const T& v1, const T& v2)> cmp = [](const T& v1, const T& v2) {
                if (v1 == v2) return 0;
                if (v1 > v2)  return 1;
                if (v1 < v2)  return -1;
            }
        ) -> Iterator
    {
        long long left = 0;
        long long right = end - begin;

        while (left <= right) {
            auto mid = left + (right - left) / 2;

            const int result = cmp (*(begin + mid), object);

            if (result == 0)
                return begin + mid;

            if (result > 0) {
                if constexpr (descending)
                    right = mid - 1;
                else
                    left = mid + 1;
            } else {
                if constexpr (descending)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }

        return end;
    }*/

    /**
     * \tparam Iterator The iterator
     * \param begin The begin iterator
     * \param end The end iterator
     * \return end iff object is not between [begin, end)
     * \param cmp needs to return true in case v1 >= v2
     * \tparam lowToHigh true if the list if ascending [ordine crescente]
     * */
    template <class Iterator, class T, bool lowToHigh>
    auto binary_search(
            Iterator begin,
            Iterator end,
            const T& object,
            Fn<bool(const T& v1, const T& v2)> cmp = [](const T& v1, const T& v2) {
                return v1 >= v2;
            }
        ) -> Iterator
    {
        long long left = 0;
        long long right = end - begin;

        Iterator result = end;

        while (left <= right) {
            auto mid = left + (right - left) / 2;

            if (*(begin + mid) == object) {
                result = begin + mid;
                break;
            }

            if (cmp(*(begin + mid), object)) {
                if constexpr (lowToHigh)
                    right = mid - 1;
                else
                    left = mid + 1;
            } else {
                if constexpr (lowToHigh)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }

        if (result == end)
            return end;

        for (;;) {
            WDebug(false, "Cicle")
            if (result == begin)
                return result;

            result --;
            if (*result != object) {
                WDebug(false, "return");
                return result + 1;
            }
        }
    }

    /**
     * \tparam Iterator iterator for the list
     * \tparam T the object contained in the list
     * \tparam lowToHigh true if the list should be order from low item to high item
     *
     * \param begin the begin of the list
     * \param end the end of the list
     * \param cmp should return true iff v1 >= v2
     * */
    template <class Iterator, class T, bool lowToHigh>
    auto createHeap(Iterator begin, Iterator end, Fn<bool(const T& v1, const T& v2)> cmp) -> bool
    {
        if (lowToHigh)
            WAbstractList::sort(begin, end, cmp);
        W_ASSERT(0);
    };
};
