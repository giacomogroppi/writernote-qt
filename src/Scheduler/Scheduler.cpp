#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler()
    : WObject(nullptr)
    , _threads()
    , _semGeneral(0)
    , _need_to_sort(false)
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
    std::vector<std::thread> thread;

    const auto functionThread = [this]() {
        for (;;) {
            if (this->execute())
                return;
        }
    };

    for (unsigned i = 0u; i < nThreads; i++) {
        _threads.append(std::thread(functionThread));
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

            if (needToDie())
                return;

            if (_timersWaiting.isEmpty())
                continue;

            WDebug(debug, "Wake up!" << _timersWaiting.getFirst()->getDuration()
                    << _timersWaiting.getFirst()->getEnd() << "size: " << _timersWaiting.size());

            if (_timersWaiting.getFirst()->getEnd() + minWait > WTimer::now()) {
                // if the current less timer starts within minWait ms we simply execute it now
                WDebug(debug, "Timer bigger than 50ms");
                continue;
            }

            auto *timer = _timersWaiting.takeFirst();

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

    WDebug(debug and false, "finish constructor");
}

auto Scheduler::isExecutionSchedulerThread() -> bool
{
    const auto id = std::this_thread::get_id();

    WMutexLocker _(instance->_lockGeneric);

    for (const auto& thread: std::as_const(instance->_threads)) {
        std::cout << thread.get_id() << std::endl;
        std::cout.flush();

        if (thread.get_id() == id) {
            return true;
        }
    }

    return false;
}

void Scheduler::joinThread(unsigned long& numberOfThreadCreated, unsigned long identifier)
{
    // if we are on a thread of the Scheduler we need to create another thread that is
    // executing another task in generic lists
    if (isExecutionSchedulerThread()) {
        WMutexLocker _(instance->_lockGeneric);
        WMutexLocker __(instance->_lockTaskFinish);
        numberOfThreadCreated ++;

        instance->_threads.append(
                std::thread([identifier] {
                    for (;;) {
                        {
                            WMutexLocker guard(instance->_lockTaskFinish);
                            if (instance->_taskFinished.contains(identifier) > 0) {
                                instance->_taskFinished.removeSingle(identifier);

                                return;
                            }
                        }

                        instance->execute();
                    }
                }
            )
        );
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
        WMutexLocker _(instance->_lockTaskFinish);
        for (unsigned long i = 0; i < task->_threadsCreated; i++) {
            instance->_taskFinished.append({task, task->getIdentifier()});
        }
    }

    task->releaseJoiner();

    if (needToDeleteLater) {
        //TODO: remove this comment and the above
        //delete task;
    }
}

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
    for (auto &t: this->_task_General) {
        t->releaseJoiner();

        if (t->isDeleteLater())
            ;
            //TODO: remove this comment and the above
            //delete t;
    }
    _lockGeneric.unlock();

    instance = nullptr;

    WDebug(debug and false, "finish destructor");
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


