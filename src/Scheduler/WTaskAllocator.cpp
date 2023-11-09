#include "WTaskAllocator.h"

static WTaskAllocator *allocator;

static constexpr auto allocateNew = [] {
    return new WTaskDataPrivate ();
};

WTaskAllocator::WTaskAllocator()
    : _allocator(allocateNew)
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
