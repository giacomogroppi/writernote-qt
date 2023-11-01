#pragma once

#include "utils/WCommonScript.h"
#include "utils/threadcount.h"
#include "testing/memtest.h"
#include "pthread.h"
#include "core/WSemaphore.h"
#include "core/pointer/UniquePtr.h"
#include "core/WVector.h"

struct DataPrivateMuThread{
    int from, to;

    // indicate the identifier for the thread
    int id;
    void *extra;
};

#define THREAD_STOP_REQUEST BIT(1)

class thread_group_sem{
private:
    int _core;
    int _flag;
    int _create;

    WSemaphore _finish;
    WSemaphore _pass;
    WSemaphore _all_finish;

    DataPrivateMuThread *_data;
    pthread_t *_thread;

    void stopThread();
public:
    thread_group_sem();
    ~thread_group_sem();

    DataPrivateMuThread *get_thread_data();

    auto get_pass_sem() -> WSemaphore&;
    auto get_finish_sem() -> WSemaphore&;
    auto get_all_finish_sem() -> WSemaphore&;

    bool needToDelete() const;

    void startLoop(void *(*function)(void *));
    void postAndWait(int create);
    int get_create() const;
    int get_max() const;

    void waitForThread(WSemaphore &sem);
    void postForThread(WSemaphore &sem);
};

force_inline bool thread_group_sem::needToDelete() const
{
    return _flag & THREAD_STOP_REQUEST;
}

force_inline WSemaphore &thread_group_sem::get_all_finish_sem()
{
    return _all_finish;
}

force_inline WSemaphore &thread_group_sem::get_pass_sem()
{
    return _pass;
}

force_inline WSemaphore &thread_group_sem::get_finish_sem()
{
    return _finish;
}

force_inline void thread_group_sem::startLoop(void *(*function)(void *))
{
    int i;
    for(i = 0; i < _core; i++){
        pthread_create(&_thread[i], nullptr, function, &_data[i]);
    }
}

force_inline thread_group_sem::thread_group_sem():
    _finish(0),
    _pass(0),
    _all_finish(0)
{
    _flag = 0;
    _core = threadCount::count();

    _thread = (pthread_t *) WMalloc(sizeof(*_thread) * _core);
    _data = (DataPrivateMuThread *) WMalloc(sizeof(*_data) * _core);
}

// when call this function data should be set
inline void thread_group_sem::postAndWait(int create)
{
    W_ASSERT(create <= get_max());
    volatile int *create_volatile = &this->_create;
    *create_volatile = create;

    this->postForThread(_pass);
    this->waitForThread(_finish);
    this->postForThread(_all_finish);
}

force_inline int thread_group_sem::get_create() const
{
    return _create;
}

inline int thread_group_sem::get_max() const
{
    return _core;
}

inline void thread_group_sem::waitForThread(WSemaphore &sem)
{
    int i;
    for (i = 0; i < _core; i++) {
        sem.acquire();
    }
}

inline void thread_group_sem::postForThread(WSemaphore &sem)
{
    int i;
    for (i = 0; i < _core; i++) {
        sem.release();
    }
}

force_inline thread_group_sem::~thread_group_sem()
{
    int i;
    stopThread();
    postForThread(_pass);

    for(i = 0; i < _core; i++){
        pthread_join(_thread[i], nullptr);
    }

    WFree(_thread);
    WFree(_data);
}

force_inline auto thread_group_sem::get_thread_data() -> DataPrivateMuThread *
{
    return _data;
}

force_inline void thread_group_sem::stopThread()
{
    _flag |= THREAD_STOP_REQUEST;
}

void DataPrivateInit();

#define DATA_PRIVATE_FLAG_SEM BIT(1)
int DataPrivateMuThreadInit(WVector<DataPrivateMuThread> &data, void *extraData, int maxThread, int to, int flag);

int DataPrivateCountThread(int numNewThread);
void DataPrivateCountThreadRelease(int numReleaseThread);

pthread_t *get_thread_max();
WVector<DataPrivateMuThread> get_data_max();

void free_thread_data(pthread_t **thread, DataPrivateMuThread **data);

[[deprecated]]
int get_thread_used();

#define START_THREAD(thread, data, count, functionToCall) \
    for(int __i = 0; __i < count; __i++){                                     \
        pthread_create(&thread[__i], nullptr, functionToCall, &data[__i]);     \
    }                                                                   \


#define JOIN_THREAD(thread, count) for(int __i = 0; __i < count; __i ++){ pthread_join(thread[__i], nullptr); }

force_inline void joinThread(pthread_t *thread, int count)
{
    int i;
    for(i = 0; i < count; i++){
        pthread_join(thread[i], nullptr);
    }
}

// semaphore
#define TRIGGER_END(thread_create, sem) for(int __i = 0; __i < thread_create; __i++){ sem_post(&sem); }
#define THREAD_SEM_FINISH(sem) sem_post(&sem)
#define JOIN_THREAD_SEM(count, sem)         \
    for(int __i = 0; __i < count; __i++){   \
        sem_wait(&sem);                     \
    }                                       \

