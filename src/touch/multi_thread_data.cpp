#include "multi_thread_data.h"
#include "utils/common_script.h"
#include "log/log_ui/log_ui.h"
#include <pthread.h>

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

int DataPrivateMuThreadInit(DataPrivateMuThread *data, cint len, cint to)
{
    int i, done, div, count;

    done = 0;
    div = div_diff(to, len);

    if(to > len){
        count = len;

        for(i = 0; i < len; i++){
            data[i].from = done;
            data[i].to = done + div;
            done += div;
        }

        data[len - 1].to = to;
    }else{
        count = 1;
        data[0].from = 0;
        data[0].to = to;
    }

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
