#pragma once

#include <functional>
#include "Scheduler/WObject.h"
#include <queue>
#include "WPool.h"
#include "core/WMutex.h"
#include "core/AtomicSafe.h"
#include "WTask.h"
#include "core/WSemaphore.h"
#include "core/WList.h"
#include <thread>
#include "core/WVector.h"

class SchedulerThreadData {
public:
    bool *thread;
};


class Scheduler final: public WObject{
private:
    WList<WTask *> _task_Main;
    WList<WTask *> _task_General;

    WVector<std::thread> _threads;

    mutable WSemaphore _semGeneral;
    mutable WSemaphore _semMain;

    mutable WMutex _lockMain;
    mutable WMutex _lockGeneric;

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

    AtomicSafe<bool> _need_to_sort;
    bool _needToDie;

public:
    explicit Scheduler();
    ~Scheduler() final;


    constexpr bool needToDie() const noexcept;

    void addTaskGeneric(WTask *task);
    static void addTaskMainThread(WTask *task);


    void exit();
};
