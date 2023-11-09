#include "Allocators.h"

#include "Scheduler/WTaskAllocator.h"

auto Allocators::init() -> void
{
    for (auto& ref: methodsInit) {
        ref();
    }
}

auto Allocators::exit() -> void
{
    for (auto &ref: methodsExit) {
        ref();
    }
}
