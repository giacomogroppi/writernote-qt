#pragma once

#include <type_traits>

class WritableAbstract {
public:
    virtual auto write (const void *data, size_t size) -> int = 0;

    /*
    virtual auto write (int object) -> int = 0;
    virtual auto write (long object) -> int = 0;
    virtual auto write (size_t object)
     */
};