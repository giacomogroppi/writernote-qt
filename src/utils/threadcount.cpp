#include "threadcount.h"
#include <thread>

uint threadCount::count()
{
    static int thread = std::thread::hardware_concurrency();
    return thread;
}
