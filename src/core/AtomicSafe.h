#pragma once

#include "WMutex.h"
#include "WMutexLocker.h"
#include <atomic>

template<class T>
using AtomicSafe = std::atomic<T>;
