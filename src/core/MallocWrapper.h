#pragma once

#include "utils/WCommonScript.h"
#include <cstdlib>

template <class T, class ...Args>
class MallocWrapper {
public:
    static auto allocClass (Args&& ...args) -> T* {
        return new T(std::forward<Args>(args)...);
    };

    static auto deallocClass (T* pointer) -> void {
        delete pointer;
    };
};


