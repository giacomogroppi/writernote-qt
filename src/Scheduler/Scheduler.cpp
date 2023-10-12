#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler()
    : WObject(nullptr)
    , _threads()
    , _semGeneral(0)
    , _needToDie(false)
    , _timersWaiting([](const WTimer *t1, const WTimer *t2) -> bool {
        return t1->getEnd() > t2->getEnd();
    })
{
    WDebug(debug and false, "Call constructor");

    const auto nThreads = numberOfThread();
    W_ASSERT(instance == nullptr);

    instance = this;

    _threads.reserve(nThreads + 1);
    //std::vector<std::thread> thread;

    const auto functionThread = [this]() {
        for (;;) {
            if (this->execute()) {
                WMutexLocker _(_lockGeneric);
                this->_idThreads.removeSingle(std::this_thread::get_id());
                return;
            }
        }
    };

    for (unsigned i = 0u; i < nThreads; i++) {
        _threads.append(std::thread(functionThread));
        _idThreads.append(_threads.last().get_id());
    }

    // start a new threads for timers
    _threads.append(std::thread([this]() {
        using namespace std;
        using namespace chrono;

        using namespace std::chrono;
        std::unique_lock<std::mutex> lk (this->_muxTimers);

        for (;;) {
            constexpr int minWait = 10;
            const chrono::duration shouldWaitFor = (_timersWaiting.isEmpty()
                    ? 160s
                    : chrono::milliseconds(
                                this->_timersWaiting.getFirst()->getEnd() -
                                WTimer::now()
                            ));

            const unsigned long lastValueEnd = _timersWaiting.isEmpty()
                    ? (0x2ul << 60)
                    : _timersWaiting.getFirst()->getEnd();

            if (shouldWaitFor.count() > minWait) {
                this->_c.wait_for(lk, shouldWaitFor, [&lastValueEnd, this] {
                    // if the list has been modified we need to reschedule the
                    // timer for std::condition_value, or we need to die
                    if (needToDie())
                        return true;

                    if (_timersWaiting.isEmpty())
                        return false;

                    if (_timersWaiting.getFirst()->getEnd() < lastValueEnd)
                        return true;
                    return false;
                });
            }

            if (needToDie()) {
                WMutexLocker _(_lockGeneric);
                this->_idThreads.removeSingle(std::this_thread::get_id());

                return;
            }

            if (_timersWaiting.isEmpty())
                continue;

            WDebug(debug, "Wake up!" << _timersWaiting.getFirst()->getDuration()
                    << _timersWaiting.getFirst()->getEnd() << "size: " << _timersWaiting.size());

            if (_timersWaiting.getFirst()->getEnd() + minWait > WTimer::now()) {
                // if the current less timer starts within minWait ms we simply execute it now
                WDebug(debug, "Timer bigger than 50ms");
                continue;
            }

            auto timer = _timersWaiting.takeFirst();

            timer->_lock.lock();

            WDebug(debug, "Execute" << timer->getDuration());

            const auto isExecutionMainThread = timer->isExecutionMainThread();
            const auto isSingleShot = timer->isSingleShot();

            auto task = SharedPtrThreadSafe<WTask>(new WTaskFunction(nullptr, [timer, isSingleShot] {
                timer->trigger();

                if (not isSingleShot)
                    timer->start();
            }, true));

            timer->_lock.unlock();

            // mutex is already locked
            if (isExecutionMainThread) {
                Scheduler::addTaskMainThread(std::move(task));
            } else {
                Scheduler::addTaskGeneric(std::move(task));
            }
        }
    }));
    _idThreads.append(_threads.last().get_id());

    WDebug(debug and false, "finish constructor");
}

auto Scheduler::isExecutionSchedulerThread() -> bool
{
    const auto id = std::this_thread::get_id();

    WMutexLocker _(instance->_lockGeneric);

    return instance->_idThreads.contains(id);
}

void Scheduler::joinThread(std::atomic<int>& numberOfThreadCreated, unsigned long identifier)
{
    // if we are on a thread of the Scheduler we need to create another thread that is
    // executing another task in generic lists
    if (isExecutionSchedulerThread()) {
        WMutexLocker _(instance->_lockGeneric);
        numberOfThreadCreated ++;

        std::thread([identifier] {
            {
                WMutexLocker _(instance->_lockGeneric);
                instance->_idThreads.append(std::this_thread::get_id());
            }

            for (;;) {
                {
                    instance->_lockTaskFinished.lock_shared();
                    WDebug(true, "Task finish: " << instance->_taskFinished.size()
                                    << "thread size:" << instance->_idThreads.size()
                                    << "waiting for: " << identifier
                                    << "array task finish:" << instance->_taskFinished
                    );

                    if (instance->_taskFinished.contains(identifier)) {
                        WDebug(true, "Contains");
                        instance->_taskFinished.removeSingle(identifier);

                        instance->_idThreads.removeSingle(std::this_thread::get_id());

                        return;
                    }

                    instance->_lockTaskFinished.unlock_shared();
                }

                instance->execute();
            }
        }).detach();
    }
}

auto Scheduler::execute() -> bool
{
    _semGeneral.acquire();

    if (this->needToDie())
        return true;

    SharedPtrThreadSafe<WTask> task;

    {
        WMutexLocker _(_lockGeneric);
        task = _task_General.takeFirst();
    }

    Scheduler::manageExecution(std::move(task));

    return false;
}

void Scheduler::manageExecution(SharedPtrThreadSafe<WTask> task)
{
    WDebug(debug and false, "Execute object" << static_cast<const void*>(&(*task)));

    const auto needToDeleteLater = task->isDeleteLater();

    task->run();

    {
        WMutexLocker _(instance->_lockTaskFinished);
        for (unsigned long i = 0; i < task->_threadsCreated; i++) {
            instance->_taskFinished.append(task->getIdentifier());
        }
    }

    task->releaseJoiner();

    if (needToDeleteLater) {
        //TODO: remove this comment and the above
        //delete task;
    }
}
#include <QThread>
Scheduler::~Scheduler()
{
    WDebug(debug and false, "Call destructor");
    _needToDieLock.lock();
    this->_needToDie = true;

    this->_semGeneral.release(_threads.size());

    _needToDieLock.unlock();

    _muxTimers.lock();
    _c.notify_all();
    _muxTimers.unlock();

    for (auto &ref: _threads)
        ref.join();

    _lockGeneric.lock();
    _lockTaskFinished.lock();
    for (auto &t: this->_task_General) {
        {
            for (unsigned long i = 0; i < t->_threadsCreated; i++) {
                instance->_taskFinished.append(t->getIdentifier());
                _semGeneral.release();
            }
        }

        t->releaseJoiner();

        if (t->isDeleteLater())
            ;
            //TODO: remove this comment and the above
            //delete t;
    }
    _lockTaskFinished.unlock();
    _lockGeneric.unlock();

    WDebug(true, "finish destructor" << _taskFinished.size());


    while (_idThreads.size())
        ;

    instance = nullptr;

    //QThread::sleep(1);

    WDebug(true, "finish destructor" << _taskFinished.size());
}

void Scheduler::createHeap()
{
    W_ASSERT(this->isHeap());
}

auto Scheduler::isHeap() const -> bool
{
    return true;
}

auto Scheduler::needToDie() const noexcept -> bool
{
    WMutexLocker locker (this->_needToDieLock);
    return this->_needToDie;
}

auto Scheduler::getInstance() -> Scheduler &
{
    W_ASSERT(instance != nullptr);
    return *instance;
}

auto Scheduler::addTimerUnsafe(WTimer *timer) -> void
{
    _timersWaiting.add(timer);
    _c.notify_all();
}

auto Scheduler::addTimer(WTimer *timer) -> void
{
    W_ASSERT(timer != nullptr);
    
    WMutexLocker _(this->_muxTimers);
    this->addTimerUnsafe(timer);

    WDebug(debug, "Add timer with end in " << timer->getEnd()
            << "and duration: " << timer->getDuration() << "current minor: " << _timersWaiting.getFirst()->getEnd());
}

auto Scheduler::removeTimer(WTimer *timer) -> void
{
    W_ASSERT(timer != nullptr);
    WMutexLocker _(this->_muxTimers);

    if (_timersWaiting.removeIfPresent(timer))
        this->_c.notify_all();
}


