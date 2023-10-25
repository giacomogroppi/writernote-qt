#pragma once

#include "utils/common_def.h"
#include "utils/WCommonScript.h"

#if defined(WIN32) || defined(WIN64)
#include <windows.h> // for Sleep
#endif

force_inline void waitTime(int ms)
{
    // TODO: use std::this_thread::sleep
#if defined(WIN32) || defined(WIN64)
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, nullptr);
#endif
}

