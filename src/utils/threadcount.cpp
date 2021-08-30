#include "threadcount.h"
#include <thread>

uint threadCount::count()
{
    return std::thread::hardware_concurrency();
}
