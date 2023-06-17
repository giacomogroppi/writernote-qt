#ifndef WRITERNOTE_WABSTRACTLIST_H
#define WRITERNOTE_WABSTRACTLIST_H

#include "utils/WCommonScript.h"
#include <algorithm>

namespace WAbstractList {
    template<class T>
    auto isSorted(const T& list) -> bool;

    template <class T>
    auto sort (T &list) -> void;

    /*
    template <class T, class K, class Writable>
    auto save (const T &list, Writable &writable) noexcept -> int
    {
        static_assert_type(list._size, int);

        if (writable.write(list._size) < 0) {
            return -1;
        }

        for (const auto &ref: std::as_const(list)) {
            if (K::save(writable, list) < 0)
                return -1;
        }
        return 0;
    }*/
};
#endif //WRITERNOTE_WABSTRACTLIST_H
