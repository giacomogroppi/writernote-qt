#pragma once

#include "Scheduler/WTaskAllocator.h"

class Allocators
{
private:
    static constexpr auto methodsInit = {
            &WTaskAllocator::init
    };

    static constexpr auto methodsExit = {
            &WTaskAllocator::exit
    };
public:
    static void init();
    static void exit();
};


