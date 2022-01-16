#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

#include "utils/common_script.h"

struct DataPrivateMuThread{
    int from, to;
    void *extra;
};

void DataPrivateInit(void);

int DataPrivateMuThreadInit(DataPrivateMuThread *data, void *extraData, cint maxThread, cint to);

int DataPrivateCountThread(int numNewThread);
void DataPrivateCountThreadRelease(int numReleaseThread);

pthread_t *get_thread_max(void);
DataPrivateMuThread *get_data_max(void);

void free_thread_data(pthread_t *thread, DataPrivateMuThread *data);

int get_thread_used();

#endif // MULTI_THREAD_DATA_H
