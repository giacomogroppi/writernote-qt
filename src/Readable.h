#pragma once

#include <type_traits>
#include <cstddef>
#include "core/Error.h"

class ReadableAbstract {
public:
    /**
     * \param to The block of memory to write
     * \param size The size of the load
     * \return &lt 0 in case of error
     * */
    virtual Error read (void *to, size_t size) const = 0;

    template <class T>
            requires (std::is_arithmetic_v<T> 
                      && sizeof (T) <= sizeof(void*)
                      && std::is_pointer<T>::value == false)
    auto read (T &value)
    {
        return read (&value, sizeof(value));
    }
};
