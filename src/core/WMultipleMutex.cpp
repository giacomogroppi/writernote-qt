#include "WMultipleMutex.h"
#include "utils/WCommonScript.h"
#include "testing/memtest.h"

WMultipleMutex::WMultipleMutex(int size, int initVal)
{
    W_ASSERT(initVal >= 0);
    W_ASSERT(initVal < size);

    this->_size = size;
    this->_sem = new WSemaphore[size];
}

WMultipleMutex::~WMultipleMutex()
{
    delete[] this->_sem;
}

int WMultipleMutex::lock(int num)
{
    W_ASSERT(num < _size);
    W_ASSERT(num >= 0);

    return this->block(num, num + 1);
}


