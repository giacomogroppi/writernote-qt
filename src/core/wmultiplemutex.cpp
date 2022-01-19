#include "wmultiplemutex.h"
#include "utils/common_script.h"

WMultipleMutex::WMultipleMutex(int maxVal, int initVal)
{
    int i;
    W_ASSERT(initVal >= 0);
    W_ASSERT(initVal < maxVal);

    this->maxVal = maxVal;
    this->mutex = (pthread_mutex_t *)malloc(sizeof(*mutex) * (maxVal + 1));

    for(i = 0; i < maxVal; i ++){
        pthread_mutex_init(&mutex[i], NULL);
    }

    for(i = maxVal; i >= initVal; i--){
        qDebug() << i;
        pthread_mutex_lock(&mutex[i]);
    }
}

WMultipleMutex::~WMultipleMutex()
{
    free(mutex);
}

int WMultipleMutex::lock(int num)
{
    W_ASSERT(num < maxVal);
    W_ASSERT(num >= 0);

    return pthread_mutex_lock(&mutex[num]);
}

// it means that the last thread is trying to free
// the last one its successes, which does not
// exist, so we return only 0
int WMultipleMutex::unlock(int n)
{
    W_ASSERT(n <= maxVal);
    W_ASSERT(n >= 0);

    // it means that the last thread is trying to free
    // the last one its successes, which does not exist,
    // so we return only 0, or the first thread is
    // trying to authorize its previous one
    if(unlikely(n == maxVal || n == -1))
        return 0;

    return pthread_mutex_unlock(&mutex[n]);
}
