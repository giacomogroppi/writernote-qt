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


class Scheduler final: public WObject
{
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

    mutable WMutex _needToDieLock;
    volatile bool _needToDie;

public:
    explicit Scheduler();
    ~Scheduler() final;

    bool needToDie() const noexcept;

    static void addTaskGeneric(WTask *task);
    static void addTaskMainThread(WTask *task);

    static
    Scheduler &getInstance();

    /**
     * This function is used to create a new task within the scheduler with "needToDeleteLater"
     * set to false, so it is up to whoever receives the task to destroy it.
     * The function pass will be executed in a generic thread
     * */
    static constexpr auto startNewTask = [] (std::function<void()> function) -> WTask * {
        WTask *task = new WTaskFunction(nullptr, std::move(function));

        task->setDestroyLater(false);

        Scheduler::addTaskGeneric(task);
        return task;
    };
};

inline void Scheduler::addTaskGeneric(WTask *task)
{
    auto &sched = Scheduler::getInstance();
    WMutexLocker _(sched._lockGeneric);
    sched._task_General.append(task);

    sched._semGeneral.release();

    sched._need_to_sort = true;
}