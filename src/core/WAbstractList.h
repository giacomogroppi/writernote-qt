#ifndef WRITERNOTE_WABSTRACTLIST_H
#define WRITERNOTE_WABSTRACTLIST_H

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

    /*
    template <class T, class K, class Writable>
    auto write (const T &list, Writable &writable) noexcept -> int
    {
        static_assert_type(list._size, int);

        if (writable.write(list._size) < 0) {
            return -1;
        }

        for (const auto &ref: std::as_const(list)) {
            if (K::write(writable, list) < 0)
                return -1;
        }
        return 0;
    }*/
};
#endif //WRITERNOTE_WABSTRACTLIST_H
