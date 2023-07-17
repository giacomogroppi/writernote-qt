#pragma once

#include "Writable.h"
#include <vector>
#include <functional>

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

    [[nodiscard]]
    auto getCurrentSize() const -> size_t;

    /**
     * \return &lt 0 in case of errors otherwise it return 0
     */
    auto merge (WritableAbstract &writable) -> int;

    /**
     * \param append Function for append data to a custom object.
     *  This function has to return &lt 0 in case of error
     * \return &lt in case "append" return an error
     * */
    auto merge (std::function<int(const void *d, size_t size)> append) -> int;
};
