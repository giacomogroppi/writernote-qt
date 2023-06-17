#pragma once

#include <type_traits>

class ReadableAbstract {
public:
    virtual int read (void *to, size_t size) = 0;
};