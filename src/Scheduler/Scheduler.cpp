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
        WSemaphore *sem =           &this->_semGeneral;
        WMutex *mux =               &this->_lockGeneric;
        WList<WTask *> *tasksHeap = &this->_task_General;

        // loop
        for (;;) {
            WTask *task;
            sem->acquire();

            if (this->needToDie())
                return;

            {
                WMutexLocker _(*mux);
                task = tasksHeap->takeFirst();
            }

            this->manageExecution(task);
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
                    ? chrono::milliseconds (100'000)
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

            WDebug(debug, "Execute" << timer->getDuration());

            timer->_lock.lock();

            auto *task = new WTaskFunction(nullptr, [timer] {
                timer->trigger();
            }, true);

            // mutex is already locked
            if (timer->isExecutionMainThread()) {
                Scheduler::addTaskMainThread(task);
            } else {
                Scheduler::addTaskGeneric(task);
            }

            WTask *taskToExecute = nullptr;

            if (not timer->isSingleShot()) {
                taskToExecute = new WTaskFunction(nullptr, [timer]() {
                    timer->start();
                }, true);
            }

            timer->_lock.unlock();

            if (taskToExecute != nullptr)
                Scheduler::addTaskGeneric(taskToExecute);
        }
    }));

    WDebug(debug and false, "finish constructor");
}

void Scheduler::manageExecution(WTask *task)
{
    WDebug(debug and false, "Execute object" << static_cast<void*>(task));

    const auto needToDeleteLater = task->isDeleteLater();

    task->run();
    task->releaseJoiner();

    if (needToDeleteLater) {
        delete task;
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
    for (auto *t: this->_task_General) {
        t->releaseJoiner();

        if (t->isDeleteLater())
            delete t;
    }
    _lockGeneric.unlock();

    instance = nullptr;

    WDebug(debug and false, "finish destructor");
}

void Scheduler::createHeap()
{
    /*
    std::make_heap(_task_General.begin(),
                   _task_General.end(),
                   [](WTask *a, WTask *b) {
        return a->getPriority() < b->getPriority();
    });
     */
    W_ASSERT(this->isHeap());
}

bool Scheduler::isHeap() const
{
    return true;
    /*
    WMutexLocker _(this->_pool_active_locker);
    const auto r = this->_task_General.at(0)->getPriority();

    for (int i = 1; i < (int)this->_task_General.size(); i++) {
        if (r < this->_task_General.at(i)->getPriority())
            return false;
    }
    return true;
*/
}

bool Scheduler::needToDie() const noexcept
{
    WMutexLocker locker (this->_needToDieLock);
    return this->_needToDie;
}

Scheduler &Scheduler::getInstance()
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


