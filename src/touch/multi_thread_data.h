#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

#include "utils/common_script.h"

struct DataPrivateMuThread{
    int from, to;

    // indicate the identifier for the thread
    int id;
    void *extra;
};

void DataPrivateInit(void);

#define DATA_PRIVATE_FLAG_SEM BIT(1)
int DataPrivateMuThreadInit(DataPrivateMuThread *data, void *extraData, cint maxThread, cint to, int flag);

int DataPrivateCountThread(int numNewThread);
void DataPrivateCountThreadRelease(int numReleaseThread);

pthread_t *get_thread_max(void);
DataPrivateMuThread *get_data_max(void);

void free_thread_data(pthread_t **thread, DataPrivateMuThread **data);

int get_thread_used();

#define START_THREAD(thread, data, count, functionToCall) \
    for(int __i = 0; __i < count; __i++){                                     \
        pthread_create(&thread[__i], NULL, functionToCall, &data[__i]);     \
    }                                                                   \


#define JOIN_THREAD(thread, count) for(int __i = 0; __i < count; __i ++){ pthread_join(thread[__i], NULL); }

force_inline void joinThread(pthread_t *thread, int count)
{
    int i;
    for(i = 0; i < count; i++){
        pthread_join(thread[i], NULL);
    }
}

// semaphore
#define TRIGGER_END(thread_create, sem) for(int __i = 0; __i < thread_create; __i++){ sem_post(&sem); }
#define THREAD_SEM_FINISH(sem) sem_post(&sem)
#define JOIN_THREAD_SEM(count, sem)         \
    for(int __i = 0; __i < count; __i++){   \
        sem_wait(&sem);                     \
    }                                       \

#endif // MULTI_THREAD_DATA_H
