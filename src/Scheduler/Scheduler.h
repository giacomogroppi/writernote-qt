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
#include "core/pointer/Pointer.h"

class Scheduler final: public WObject
{
public:
    template <class P>
    using Ptr = Pointer<P>;

    /**
     * \return True if there is no instance of scheduler in the
     * all application
     */
    static auto isStop() noexcept -> bool;

    static constexpr auto debug = false;
    WList<Ptr<WTask>> _task_General;

    WVector<std::thread> _threads;
    WVector<std::thread::id> _idThreads;

    mutable std::condition_variable _conditionalVariableTasks;
    mutable std::mutex _lockGeneric;

    void initMainThread();
    void endMainThread();

    void createHeap();

    bool isHeap() const;

    volatile bool _needToDie;

    WHeap<WTimer*, true> _timersWaiting;
    std::mutex _muxTimers;
    std::condition_variable _conditionalVariableTimers;

    static void manageExecution (Ptr<WTask> task);
    static auto numberOfThread () -> Unsigned;

    /**
     * \brief This method needs to be call in the main function, when we are creating
     * a dedicated thread for manage timers.
     * It's possible to call this method only once
     * */
    void timerFunction();

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
    static void addTaskGeneric(Ptr<WTask> task);

    /**
     * \brief The implementation of this method depends on the framework used by writernote.
     *  In any case this method adheres to a FIFO execution order,
     *  ensuring that the task that arrives first will be executed first.
     * \param task The task that need to be executed in the main thead
     * */
    static void addTaskMainThread(Ptr<WTask> task);

    static auto getInstance() -> Scheduler &;

    /**
     * This function is used to create a new task within the scheduler with "needToDeleteLater"
     * set to false, so it is up to whoever receives the task to destroy it.
     * The function pass will be executed in a generic thread
     * */
    static auto startNewTask (Fn<void()> function) -> Ptr<WTask> {
        Ptr<WTask> task (new WTaskFunction(nullptr, WTask::NotDeleteLater, std::move(function)));

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

    static void joinThread(AtomicSafe<bool> &hasFinish);

    friend class WTask;

    static bool isExecutionSchedulerThread();

    /**
     * \return true iff we need to die
     * */
    template <int timeWait = -1>
    bool execute();
};

inline void Scheduler::addTaskGeneric(Ptr<WTask> task)
{
    task->beforeRun();

    if constexpr (WUtils::debug_enable()) {
        if (task->isDeleteLater())
            W_ASSERT(task.numberOfRef() == 1);
    }

    auto &sched = Scheduler::getInstance();
    WMutexLocker _(sched._lockGeneric);
    sched._task_General.append(std::move(task));

    sched._conditionalVariableTasks.notify_one();
}

