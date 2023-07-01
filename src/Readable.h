#pragma once

#include <type_traits>
#include <cstddef>

class ReadableAbstract {
public:
    /**
     * \param to The block of memory to write
     * \param size The size of the load
     * \return &lt 0 in case of error
     * */
    virtual int read (void *to, size_t size) const = 0;

    template <class T>
            requires (std::is_arithmetic_v<T> && sizeof (T) <= sizeof(void*))
    auto read (T &value)
    {
        return read (&value, sizeof(value));
    }
};