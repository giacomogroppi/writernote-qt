#include "MemWritable.h"
#include "utils/WCommonScript.h"

MemWritable::MemWritable()
        : _size(0u)
        , _internalStack(0)
        , _allocatedMemory()
{
    _allocatedMemory.push_back(malloc(MemWritable::sizePage));
}

auto MemWritable::getCurrentSize() const -> size_t
{
    return this->_size;
}

auto MemWritable::merge(WritableAbstract &writable) -> int
{
    return this->merge([&writable](const void *d, size_t size) -> int {
        return writable.write(d, size);
    });
}

int MemWritable::write(const void *data, size_t size)
{
    if (size == 0)
        return 0;

    const auto originalSize = size;
    size -= this->writeUntilEnd(data, size);

    if (size > 0) {
        while (size >= MemWritable::sizePage) {
            size -= this->writeInDedicated(static_cast<const char*>(data) + originalSize - size, size);
        }

        if (size != 0)
            this->writeFromZero(static_cast<const char*>(data) + originalSize - size, size);
    }

    return 0;
}

MemWritable::~MemWritable()
{
    for (auto *page: _allocatedMemory) {
        free(page);
    }
}

auto MemWritable::merge(const Fn<int(const void *, size_t)>& append) -> int
{
    // write all the blocks
    for (auto iter = _allocatedMemory.cbegin(); iter < _allocatedMemory.cend() - 1; iter ++) {
        if (append(*iter, MemWritable::sizePage) < 0)
            return -1;
    }

    // write the last object
    if (append(*(_allocatedMemory.rbegin()), _internalStack) < 0)
        return -1;

    return 0;
}

auto MemWritable::writeUntilEnd(const void *from, size_t size) -> size_t
{
    const size_t byteToWrite = std::min(MemWritable::sizePage - _internalStack, size);
    WCommonScript::WMemcpy(getData() + _internalStack, static_cast<const char*>(from), byteToWrite);

    _size += byteToWrite;
    _internalStack += byteToWrite;

    if (_internalStack == MemWritable::sizePage) {
        _allocatedMemory.push_back(malloc (MemWritable::sizePage));
        _internalStack = 0u;
    }

    return byteToWrite;
}

auto MemWritable::writeInDedicated(const void *from, size_t size) -> size_t
{
    W_ASSERT(size >= MemWritable::sizePage);
    W_ASSERT(_internalStack == 0u);

    WCommonScript::WMemcpy(getData(), from, MemWritable::sizePage);

    _allocatedMemory.push_back(malloc(MemWritable::sizePage));
    _size += MemWritable::sizePage;

    return MemWritable::sizePage;
}

auto MemWritable::writeFromZero(const void *from, size_t size) -> size_t
{
    W_ASSERT(size < MemWritable::sizePage);
    W_ASSERT(_internalStack == 0u);

    WCommonScript::WMemcpy(getData(), from, size);

    _size += size;
    _internalStack += size;

    return size;
}


