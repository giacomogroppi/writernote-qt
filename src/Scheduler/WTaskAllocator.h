#pragma once

#include "core/Allocator.h"

#include "WTask.h"

class WTaskAllocator
{
private:
    Allocator<WTaskDataPrivate, false, true> _allocator;

    explicit WTaskAllocator();
    ~WTaskAllocator();
public:

    static auto get() -> WTaskDataPrivate*;
    static auto put(WTaskDataPrivate *object) -> void;

private:
    static void init();
    static void exit();

    friend class Allocators;
};
