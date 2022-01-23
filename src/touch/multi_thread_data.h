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

void free_thread_data(pthread_t *thread, DataPrivateMuThread *data);

int get_thread_used();

force_inline void start_thread(pthread_t *thread, DataPrivateMuThread *data, int count, void *(*functionToCall)(void *))
{
    int i;
    for(i = 0; i < count; i++){
        pthread_create(&thread[i], NULL, functionToCall, &data[i]);
    }
}

force_inline void joinThread(pthread_t *thread, int count)
{
    int i;
    for(i = 0; i < count; i++){
        pthread_join(thread[i], NULL);
    }
}

#endif // MULTI_THREAD_DATA_H
