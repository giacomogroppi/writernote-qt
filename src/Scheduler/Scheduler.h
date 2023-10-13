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
#include "core/WHeap.h"
#include "WTimer.h"
#include "core/WElement.h"
#include "core/WMap.h"
#include "core/pointer/SharedPtrThreadSafe.h"
#include <shared_mutex>

class Scheduler final: public WObject
{
private:
    static constexpr auto debug = false;
    WList<SharedPtrThreadSafe<WTask>> _task_General;

    WVector<std::thread> _threads;
    WVector<std::thread::id> _idThreads;

    mutable WSemaphore _semGeneral;
    mutable WMutex _lockGeneric;

    void createHeap();

    bool isHeap() const;

    mutable WMutex _needToDieLock;
    volatile bool _needToDie;

    WHeap<WTimer*, true> _timersWaiting;
    std::mutex _muxTimers;
    std::condition_variable _c;

    static void manageExecution (SharedPtrThreadSafe<WTask> task);
    static auto numberOfThread () -> Unsigned;

public:
    explicit Scheduler();
    ~Scheduler() final;

    bool needToDie() const noexcept;

    /**
     * \brief Mutex lock and free by tasks are allowed since if a task start the execution (lock the resource)
     *  sooner or later it will release it.
     *
     *  A semaphore that is released by a task can bring to a deadlock because if all tasks are waiting
     *  there are not thread available for executing that particular task.
     *
     * It's important that a task does not lock on resource that is released only
     *  by one task managed by the scheduler.
     *
     *  The order of execution is not guaranteed
     * \param task A task that need to be executed in a async thread.
     * */
    static void addTaskGeneric(const SharedPtrThreadSafe<WTask>& task);

    /**
     * \brief The implementation of this method depends on the framework used by writernote.
     *  In any case this method adheres to a FIFO execution order,
     *  ensuring that the task that arrives first will be executed first.
     * \param task The task that need to be executed in the main thead
     * */
    static void addTaskMainThread(SharedPtrThreadSafe<WTask> task);

    static
    auto getInstance() -> Scheduler &;

    /**
     * This function is used to create a new task within the scheduler with "needToDeleteLater"
     * set to false, so it is up to whoever receives the task to destroy it.
     * The function pass will be executed in a generic thread
     * */
    static constexpr auto startNewTask = [] (Fn<void()> function) -> SharedPtrThreadSafe<WTask> {
        SharedPtrThreadSafe<WTask> task(new WTaskFunction(nullptr, std::move(function)));

        task->setDestroyLater(false);

        Scheduler::addTaskGeneric(task);
        return task;
    };

    /**
     * \param timer The timer to schedule
     * */
    auto addTimer (WTimer *timer) -> void;

    auto removeTimer (WTimer *timer) -> void;

private:
    /**
     * This method is NOT thread save
     */
    auto addTimerUnsafe (WTimer *timer) -> void;

    static void joinThread(volatile bool &hasFinish);

    friend class WTask;

    static bool isExecutionSchedulerThread() ;

    /**
     * \return true iff we need to die
     * */
    template <int timeWait = -1>
    bool execute();
};

inline void Scheduler::addTaskGeneric(const SharedPtrThreadSafe<WTask>& task)
{
    auto &sched = Scheduler::getInstance();
    WMutexLocker _(sched._lockGeneric);
    sched._task_General.append(task);

    sched._semGeneral.release();
}

