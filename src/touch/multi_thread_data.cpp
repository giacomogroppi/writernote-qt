#include "multi_thread_data.h"
#include "utils/WCommonScript.h"
#include "log/log_ui/log_ui.h"
#include "utils/threadcount.h"
#include <pthread.h>
#include "testing/memtest.h"
#include "core/WMutex.h"
#include "core/WVector.h"

static WMutex mutex_thread_write;
void DataPrivateInit()
{
#ifdef DEBUGINFO

    static int call = 0;
    if(call){
        NAME_LOG_EXT->write("DataPrivateInit call an other time after initialization", log_ui::possible_bug);
        std::abort();
    }
    call = 1;

#endif
}

static void ctrlThread(WVector<DataPrivateMuThread> &data, int create)
{
#ifdef DEBUGINFO
    int i, count;
    WVector<int> tmp;

    for(i = 0; i < create - 1; i++){
        if(data[i].to != data[i+1].from)
            std::abort();

        count = data[i].from;
        for (; count < data[i].to; count ++) {
            if (tmp.indexOf(count).isValid())
                std::abort();
            tmp.append(count);
        }
    }
#else
    (void)(data);
    (void)(create);
#endif
}

int DataPrivateMuThreadInit(
        WVector<DataPrivateMuThread>    &data,
        void                            *extraData,
        cint                            maxThread,
        cint                            to,
        int                             flag)
{
    int i, done, div, count;

    done = 0;
    div = WUtils::div_diff(to, maxThread);

    for (i = 0; i < maxThread and flag & DATA_PRIVATE_FLAG_SEM; i++) {
        data[i].id = i;
    }

    if (to > maxThread) {
        count = maxThread;

        for (i = 0; i < maxThread; i++) {
            data[i].from = done;
            data[i].to = done + div;
            data[i].extra = extraData;

            done += div;
        }

        data[maxThread - 1].to = to;
    } else {
        count = 1;
        data[0].from = 0;
        data[0].to = to;
        data[0].extra = extraData;
    }

    ctrlThread(data, count);

    return count;
}

static int threadLast = 240;
int DataPrivateCountThread(int newThread)
{
    int ret;

    WMutexLocker guard(mutex_thread_write);

    if (threadLast - newThread > 0) {
        ret = newThread;
        goto release;
    }

    if (!threadLast) {
        ret = 0;
        goto release;
    }

    ret = threadLast;

    release:

    threadLast -= ret;

    return ret;
}

void DataPrivateCountThreadRelease(int releaseThread)
{
    WMutexLocker guard(mutex_thread_write);

    threadLast -= releaseThread;
}

pthread_t *get_thread_max()
{
    int thread = threadCount::count();
    auto * data = (pthread_t *)WMalloc(sizeof(pthread_t) * thread);
    return data;
}

auto get_data_max() -> WVector<DataPrivateMuThread>
{
    int thread = threadCount::count();
    WVector<DataPrivateMuThread> result(thread);

    for (int i = 0; i < thread; i++)
        result.append(DataPrivateMuThread());

    return result;
}

void free_thread_data(pthread_t **thread, DataPrivateMuThread **data)
{
    W_ASSERT(thread);
    W_ASSERT(data);

    W_ASSERT(*thread);
    W_ASSERT(*data);

    free(*thread);
    free(*data);

    *thread = nullptr;
    *data = nullptr;
}

int get_thread_used()
{
    return threadCount::count();
}
