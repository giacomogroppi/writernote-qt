#ifndef WRITERNOTE_WABSTRACTLIST_H
#define WRITERNOTE_WABSTRACTLIST_H

#include "utils/WCommonScript.h"
#include <algorithm>

namespace WAbstractList {
    template<class T>
    bool isSorted(const T& list);

    template <class T>
    void sort (T &list);
};
#endif //WRITERNOTE_WABSTRACTLIST_H
