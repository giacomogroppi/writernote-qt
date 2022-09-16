#include "WMultipleMutex.h"
#include "utils/common_script.h"
#include "testing/memtest.h"

WMultipleMutex::WMultipleMutex(int size, int initVal)
{
    int i;
    W_ASSERT(initVal >= 0);
    W_ASSERT(initVal < size);

    this->size = size;
    this->sem = (sem_t *)WMalloc(sizeof(*sem) * (size + 1));

    // it already set all sem to 0 (lock)
    for(i = 0; i < size; i ++){
        sem_init(&sem[i], 0, 0);
    }
}

WMultipleMutex::~WMultipleMutex()
{
    int i;
    for(i = 0; i < size; i++)
        sem_destroy(&sem[i]);

    WFree(sem);

    sem = 0;
}

int WMultipleMutex::lock(int num)
{
    W_ASSERT(num < size);
    W_ASSERT(num >= 0);

    return this->block(num, num + 1);
}


