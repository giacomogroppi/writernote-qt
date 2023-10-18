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

    auto read (void *to, size_t size) const -> Error final;

    auto setData(const void *data, size_t max) -> MemReadable &;

    auto isAtEnd() const noexcept -> bool;
    auto getSeek() const noexcept -> size_t;
    auto getMax() const noexcept -> size_t;
};
