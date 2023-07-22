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
    auto sort (T &list) -> void
    {
        std::sort(list.begin(), list.end());
    }


    /**
     * \tparam Iterator The iterator
     * \param begin The begin iterator
     * \param end The end iterator
     * \return end iff object is not between [begin, end)
     * \param cmp needs to return true iff v1 >= v2
     * */
    template <class Iterator, class T>
    auto binary_search(
            Iterator begin,
            Iterator end,
            const T& object,
            auto cmp = [](const T& v1, const T& v2) {
                return v1 >= v2;
            }
        ) -> Iterator
    {
        static_assert(0);
    }
};
