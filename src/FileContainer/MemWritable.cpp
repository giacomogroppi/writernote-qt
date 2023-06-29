#include "MemWritable.h"
#include "utils/WCommonScript.h"

MemWritable::MemWritable()
        : _size(0)
        , _internalStack(0)
        , _allocatedMemory()
{
}

auto MemWritable::getCurrentSize() const -> size_t
{
    return this->_size;
}

auto MemWritable::merge(WritableAbstract &writable) -> int
{
    // write all the blocks
    for (auto iter = _allocatedMemory.cbegin(); iter < _allocatedMemory.cend() - 1; iter ++) {
        if (writable.write(*iter, MemWritable::sizePage) < 0)
            return -1;
    }

    // write the last object
    if (writable.write(*(_allocatedMemory.rbegin()), _internalStack) < 0)
        return -1;

    return 0;
}

int MemWritable::write(const void *data, size_t size)
{
    size_t internalDistance = 0;
    while (size >= MemWritable::sizePage) {
        // add a new memory block as penultimate, because we want to leave _internalStack unchanged
        auto position = _allocatedMemory.end() - 2;

        this->_allocatedMemory.insert(position, malloc (MemWritable::sizePage));

        WCommonScript::WMemcpy(
                *position,
                static_cast<const char*>(data) + internalDistance,
                MemWritable::sizePage
        );

        internalDistance += MemWritable::sizePage;
        size -= MemWritable::sizePage;
        this->_size += MemWritable::sizePage;
    }

    if (_size + _internalStack >= MemWritable::sizePage * _allocatedMemory.size())
        _allocatedMemory.push_back(malloc(MemWritable::sizePage));

    if (size != 0) {
        WCommonScript::WMemcpy(
                static_cast<char *>(*_allocatedMemory.rbegin()) + _internalStack,
                static_cast<const char *>(data) + internalDistance,
                size
        );
        this->_internalStack += size;
        this->_size += size;
    }

    return 0;
}

MemWritable::~MemWritable()
{
    for (auto *page: _allocatedMemory) {
        free(page);
    }
}


