#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

#include "utils/common_script.h"

struct DataPrivateMuThread{
    int from, to;
};

void DataPrivateInit(void);

int DataPrivateMuThreadInit(DataPrivateMuThread *data, cint len, cint to);

int DataPrivateCountThread(int numNewThread);
void DataPrivateCountThreadRelease(int numReleaseThread);

#endif // MULTI_THREAD_DATA_H
