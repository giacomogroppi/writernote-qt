#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

struct DataPrivateMuThread{
    int from, to;
};

void init(DataPrivateMuThread *data, int len);

#endif // MULTI_THREAD_DATA_H
