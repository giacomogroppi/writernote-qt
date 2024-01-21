#include "WTaskAllocator.h"
#include "core/MallocWrapper.h"

static WTaskAllocator *allocator;

WTaskAllocator::WTaskAllocator()
    : _allocator(MallocWrapper<WTaskDataPrivate>::allocClass,
                 MallocWrapper<WTaskDataPrivate>::deallocClass)
{

}

auto WTaskAllocator::get() -> WTaskDataPrivate*
{
    return allocator->_allocator.get();
}

auto WTaskAllocator::put(WTaskDataPrivate *object) -> void
{
    if (!allocator) {
        delete object;
    } else {
        allocator->_allocator.put(object);
    }
}

WTaskAllocator::~WTaskAllocator() = default;

void WTaskAllocator::init()
{
    W_ASSERT(allocator == nullptr);
    allocator = new WTaskAllocator;
}

void WTaskAllocator::exit()
{
    delete allocator;
    allocator = nullptr;
}
