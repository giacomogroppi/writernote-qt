#ifndef MULTI_THREAD_DATA_H
#define MULTI_THREAD_DATA_H

#include "utils/common_script.h"
#include "utils/threadcount.h"
#include "semaphore.h"
#include "pthread.h"

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

    sem_t _finish;
    sem_t _pass;
    sem_t _all_finish;

    DataPrivateMuThread *_data;
    pthread_t *_thread;

    void stopThread();
public:
    thread_group_sem();
    ~thread_group_sem();

    DataPrivateMuThread *get_thread_data();

    sem_t *get_pass_sem();
    sem_t *get_finish_sem();
    sem_t *get_all_finish_sem();

    bool needToDelete() const;

    void startLoop(void *(*function)(void *));
    void postAndWait(int create);
    int get_create() const;
    int get_max() const;

    void waitForThread(sem_t *sem);
    void postForThread(sem_t *sem);
};

force_inline bool thread_group_sem::needToDelete() const
{
    return _flag & THREAD_STOP_REQUEST;
}

force_inline sem_t *thread_group_sem::get_all_finish_sem()
{
    return &_all_finish;
}

force_inline sem_t *thread_group_sem::get_pass_sem()
{
    return &_pass;
}

force_inline sem_t *thread_group_sem::get_finish_sem()
{
    return &_finish;
}

force_inline void thread_group_sem::startLoop(void *(*function)(void *))
{
    int i;
    for(i = 0; i < _core; i++){
        pthread_create(&_thread[i], NULL, function, &_data[i]);
    }
}

force_inline thread_group_sem::thread_group_sem()
{
    _flag = 0;
    _core = threadCount::count();

    _thread = (typeof(_thread)) malloc(sizeof(*_thread) * _core);
    _data = (typeof(_data)) malloc(sizeof(*_data) * _core);

    sem_init(&_finish, 0, 0);
    sem_init(&_pass, 0, 0);
    sem_init(&_all_finish, 0, 0);
}

// when call this function data should be set
inline void thread_group_sem::postAndWait(int create)
{
    W_ASSERT(create <= get_max());
    volatile int *__create = &this->_create;
    *__create = create;

    this->postForThread(&_pass);
    this->waitForThread(&_finish);
    this->postForThread(&_all_finish);
}

force_inline int thread_group_sem::get_create() const
{
    return _create;
}

inline int thread_group_sem::get_max() const
{
    return _core;
}

inline void thread_group_sem::waitForThread(sem_t *sem)
{
    int i;
    for(i = 0; i < _core; i++){
        sem_wait(sem);
    }
}

inline void thread_group_sem::postForThread(sem_t *sem)
{
    int i;
    W_ASSERT(sem);
    for(i = 0; i < _core; i++){
        sem_post(sem);
    }
}

force_inline thread_group_sem::~thread_group_sem()
{
    int i;
    stopThread();
    postForThread(&_pass);

    for(i = 0; i < _core; i++){
        pthread_join(_thread[i], NULL);
    }

    free(_thread);

    sem_destroy(&_finish);
    sem_destroy(&_pass);
    sem_destroy(&_all_finish);

}

force_inline DataPrivateMuThread *thread_group_sem::get_thread_data()
{
    return _data;
}

force_inline void thread_group_sem::stopThread()
{
    _flag |= THREAD_STOP_REQUEST;
}

void DataPrivateInit(void);

#define DATA_PRIVATE_FLAG_SEM BIT(1)
int DataPrivateMuThreadInit(DataPrivateMuThread *data, void *extraData, cint maxThread, cint to, int flag);

int DataPrivateCountThread(int numNewThread);
void DataPrivateCountThreadRelease(int numReleaseThread);

pthread_t *get_thread_max(void);
DataPrivateMuThread *get_data_max(void);

void free_thread_data(pthread_t **thread, DataPrivateMuThread **data);

int get_thread_used();

#define START_THREAD(thread, data, count, functionToCall) \
    for(int __i = 0; __i < count; __i++){                                     \
        pthread_create(&thread[__i], NULL, functionToCall, &data[__i]);     \
    }                                                                   \


#define JOIN_THREAD(thread, count) for(int __i = 0; __i < count; __i ++){ pthread_join(thread[__i], NULL); }

force_inline void joinThread(pthread_t *thread, int count)
{
    int i;
    for(i = 0; i < count; i++){
        pthread_join(thread[i], NULL);
    }
}

// semaphore
#define TRIGGER_END(thread_create, sem) for(int __i = 0; __i < thread_create; __i++){ sem_post(&sem); }
#define THREAD_SEM_FINISH(sem) sem_post(&sem)
#define JOIN_THREAD_SEM(count, sem)         \
    for(int __i = 0; __i < count; __i++){   \
        sem_wait(&sem);                     \
    }                                       \

#endif // MULTI_THREAD_DATA_H
