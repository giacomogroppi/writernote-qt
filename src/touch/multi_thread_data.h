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

#endif // MULTI_THREAD_DATA_H
