#include "threadcount.h"
#include <thread>

int threadCount::count()
{
    static int thread = static_cast<int>(std::thread::hardware_concurrency());
    return thread;
}
