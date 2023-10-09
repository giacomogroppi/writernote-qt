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

class Scheduler final: public WObject
{
private:
    static constexpr auto debug = false;
    WList<WTask *> _task_General;

    WVector<std::thread> _threads;

    mutable WSemaphore _semGeneral;

    mutable WMutex _lockGeneric;

    void createHeap();

    bool isHeap() const;

    AtomicSafe<bool> _need_to_sort;

    mutable WMutex _needToDieLock;
    volatile bool _needToDie;

    WHeap<WTimer*, true> _timersWaiting;
    //WSemaphore _semTimers;
    std::mutex _muxTimers;
    std::condition_variable _c;

    static void manageExecution (WTask *task);

    static auto numberOfThread () -> Unsigned;

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
    static constexpr auto startNewTask = [] (Fn<void()> function) -> WTask * {
        WTask *task = new WTaskFunction(nullptr, std::move(function));

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
     * */
    auto addTimerUnsafe (WTimer *timer) -> void;
};

inline void Scheduler::addTaskGeneric(WTask *task)
{
    auto &sched = Scheduler::getInstance();
    WMutexLocker _(sched._lockGeneric);
    sched._task_General.append(task);

    sched._semGeneral.release();

    sched._need_to_sort = true;
}
