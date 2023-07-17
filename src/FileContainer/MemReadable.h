#pragma once

#include "Readable.h"
#include <vector>

class MemReadable final: public ReadableAbstract
{
    const char *_data;
    mutable size_t _seek;
    size_t _max;
public:
    MemReadable();
    explicit MemReadable(const void *data, size_t max);

    auto read (void *to, size_t size) const -> int final;

    MemReadable &setData(const void *data, size_t max);
};
