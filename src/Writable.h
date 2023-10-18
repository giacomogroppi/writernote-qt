#pragma once

#include <type_traits>
#include <cstddef>
#include "core/Error.h"

class WritableAbstract {
public:
    virtual auto write (const void *data, size_t size) -> Error = 0;

    template <class T>
            requires (std::is_arithmetic_v<T> && sizeof (T) <= sizeof(long))
    auto write (T d) -> Error
    {
        return write (&d, sizeof(d));
    }
};