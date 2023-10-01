#pragma once

#include "utils/WCommonScript.h"
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

    auto getData() -> char* {
        const auto size = _allocatedMemory.size();
        return static_cast<char*>(
                    this->_allocatedMemory[size - 1]
                );
    } ;

    /**
     * \return The number of byte written
     */
    auto writeUntilEnd(const void *from, size_t size) -> size_t;

    /**
     * \brief It's required that the _internalStack is zero and there is already a
     *  new memory allocated
     * */
    auto writeInDedicated(const void* from, size_t size) -> size_t;

    /**
     * \brief It's required that the _internalStack is zero and there is already a
     *  new memory allocated
     * */
    auto writeFromZero(const void *from, size_t size) -> size_t;
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
     // TODO: change to UnsignedLong
    auto merge (const Fn<int(const void *d, size_t size)>& append) -> int;

    auto currentSize() -> size_t { return _size; }
};
