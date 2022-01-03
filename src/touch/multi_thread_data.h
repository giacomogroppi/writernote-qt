#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

struct DataPrivateMuThread{
    int from, to;
};

int DataPrivateMuThreadInit(DataPrivateMuThread *data, int len, int to);

#endif // MULTI_THREAD_DATA_H
