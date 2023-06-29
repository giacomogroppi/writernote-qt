#pragma once

#include <type_traits>
#include <cstddef>

class WritableAbstract {
public:
    virtual auto write (const void *data, size_t size) -> int = 0;

    template <class T>
            requires (std::is_arithmetic_v<T> && sizeof (T) <= sizeof(long))
    auto write (T d) -> int
    {
        return write (&d, sizeof(d));
    }
};