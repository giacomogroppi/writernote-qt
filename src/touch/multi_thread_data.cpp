#include "multi_thread_data.h"
#include "utils/common_script.h"
#include "log/log_ui/log_ui.h"
#include "utils/threadcount.h"
#include <pthread.h>
#include "testing/memtest.h"

static pthread_mutex_t mutex_thread_write;
void DataPrivateInit(void)
{
#ifdef DEBUGINFO

    static int call = 0;
    if(call){
        NAME_LOG_EXT->write("DataPrivateInit call an other time after initialization", log_ui::possible_bug);
        std::abort();
    }
    call = 1;

#endif

    pthread_mutex_init(&mutex_thread_write, NULL);
}

static void ctrlThread(DataPrivateMuThread *data, int create)
{
#ifdef DEBUGINFO
    int i, count;
    QVector<int> tmp;

    for(i = 0; i < create - 1; i++){
        if(data[i].to != data[i+1].from)
            std::abort();

        count = data[i].from;
        for(; count < data[i].to; count ++){
            if(tmp.indexOf(count) != -1)
                std::abort();
            tmp.append(count);
        }
    }
#else
    Q_UNUSED(data);
    Q_UNUSED(create);
#endif
}

int DataPrivateMuThreadInit(
        DataPrivateMuThread     *data,
        void                    *extraData,
        cint                    maxThread,
        cint                    to,
        int                     flag)
{
    int i, done, div, count;

    done = 0;
    div = div_diff(to, maxThread);

    for(i = 0; i < maxThread; i++){
        if(flag & DATA_PRIVATE_FLAG_SEM){
            data[i].id = i;
        }
    }

    if(to > maxThread){
        count = maxThread;

        for(i = 0; i < maxThread; i++){
            data[i].from = done;
            data[i].to = done + div;
            data[i].extra = extraData;

            done += div;
        }

        data[maxThread - 1].to = to;
    }else{
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

    pthread_mutex_lock(&mutex_thread_write);

    if(likely(threadLast - newThread > 0)){
        ret = newThread;
        goto release;
    }
    if(!threadLast){
        ret = 0;
        goto release;
    }

    ret = threadLast;

    release:

    threadLast -= ret;

    pthread_mutex_unlock(&mutex_thread_write);
    return ret;
}

void DataPrivateCountThreadRelease(int releaseThread)
{
    pthread_mutex_lock(&mutex_thread_write);

    threadLast -= releaseThread;

    pthread_mutex_unlock(&mutex_thread_write);
}

pthread_t *get_thread_max()
{
    int thread = threadCount::count();
    pthread_t * data = (pthread_t *)WMalloc(sizeof(pthread_t) * thread);
    return data;
}

DataPrivateMuThread *get_data_max()
{
    int thread = threadCount::count();
    DataPrivateMuThread * data = (DataPrivateMuThread *)WMalloc(sizeof(DataPrivateMuThread) * thread);
    return data;
}


void free_thread_data(pthread_t **thread, DataPrivateMuThread **data)
{
    W_ASSERT(thread);
    W_ASSERT(data);

    W_ASSERT(*thread);
    W_ASSERT(*data);

    WFree(*thread);
    WFree(*data);

    *thread = NULL;
    *data = NULL;
}

int get_thread_used()
{
    return threadCount::count();
}
