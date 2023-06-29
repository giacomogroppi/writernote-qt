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


};
#endif //WRITERNOTE_WABSTRACTLIST_H
