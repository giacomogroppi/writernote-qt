#pragma once

#include "Writable.h"
#include <vector>

class MemWritable final: public WritableAbstract
{
private:
    size_t _size;
    size_t _internalStack;
    std::vector<void *> _allocatedMemory;

    static constexpr size_t sizePage = 4096;
public:
    MemWritable();
    ~MemWritable();

    auto write (const void *data, size_t size) -> int final;
    auto getCurrentSize() const -> size_t;

    /**
     * \return -1 in case of errors otherwise it return 0
     */
    int merge (WritableAbstract &writable);
};
