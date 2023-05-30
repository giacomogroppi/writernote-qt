#pragma once

#include <functional>
#include "Scheduler/WObject.h"
#include <queue>
#include "WPool.h"
#include "core/WMutex.h"
#include "core/AtomicSafe.h"
#include "WTask.h"
#include "core/WSemaphore.h"
#include <thread>

class SchedulerThreadData {
public:
    bool *thread;
};


class Scheduler final: public WObject{
private:
    WVector<WTask *> _pools_main_thread;
    WVector<WTask *> _pools_not_active;
    WVector<std::thread> _threads;

    mutable WSemaphore _sem;
    mutable WSemaphore _semMain;

    mutable WMutex _lockMain;
    mutable WMutex _lock_heap;

     //@ requires
     //@   _pools is locked
     //@ ensures
     //@  (\forall int i; 0 <= i < _pools.size();
     //@      _pools[0]->getPriority() >= _pools[i]->getPriority())
     //
    void createHeap();

     //@ requires
     //@   _pool_locker is not locked
     //@ ensures
     //@   (*_pool_locker is not locked*) &&
     //@   \result <==> (\forall int i; 0 <= i < _pools.size();
     //@       _pools.at(0)->getPriority() >= _pools.at(i)->getPriority())
     //
    bool is_heap() const;

    void loop(int index);

    AtomicSafe<bool> _need_to_sort;
    bool _needToDie;

public:
    explicit Scheduler(WObject *parent = nullptr);
    ~Scheduler() final;

    void addTask(WTask *task);

    constexpr bool needToDie() const noexcept;

    void exit();
};

