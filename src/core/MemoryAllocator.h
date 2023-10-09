#pragma once

#include <stdio.h>

class MemoryAllocator
{
private:
    void registerMemory(void* address, const char* row, const char* file);
public:

    static void create();
    static void destroy();

    static void* malloc(size_t size);
    static void* free();

};
