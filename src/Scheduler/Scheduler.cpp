#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

static WMutex printDebug;
#define print(messages) \
    do {                \
        WMutexLocker __guard(printDebug);       \
        std::cout << messages;                  \
        std::cout.flush();                      \
    } while (0)

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
    _idThreads.reserve(nThreads);

    const auto functionThread = [this]() {
        for (;;) {
            if (this->execute()) {
                return;
            }
        }
    };

    for (unsigned i = 0u; i < nThreads; i++) {
        _threads.append(std::thread(functionThread));
        _idThreads.append(_threads.at(i).get_id());
    }

    // start a new threads for timers
    _threads.append(std::thread([this]() {
        using namespace std;
        using namespace chrono;

        using namespace std::chrono;
        std::unique_lock<std::mutex> lk (this->_muxTimers);

        for (;;) {
            constexpr int minWait = 5;
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

            auto timer = _timersWaiting.takeFirst();

            timer->_lock.lock();

            WDebug(debug, "Execute" << timer->getDuration());

            const auto isExecutionMainThread = timer->isExecutionMainThread();
            const auto isSingleShot = timer->isSingleShot();

            auto task = Scheduler::Ptr<WTask>(new WTaskFunction(nullptr, [timer, isSingleShot] {
                timer->trigger();

                if (isSingleShot)
                    timer->setActive(false);
                else
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

    WAbstractList::sort(_idThreads.begin(), _idThreads.end());

    WDebug(debug and false, "finish constructor");
}

auto Scheduler::isExecutionSchedulerThread() -> bool
{
    const auto id = std::this_thread::get_id();

    /*
    for (const auto& i: instance->_idThreads)
        if (i == id)
            return true;
    return false;
    */

    return WAbstractList::binary_search(
            instance->_idThreads.constBegin(),
            instance->_idThreads.constEnd(),
            id
    ) != instance->_idThreads.constEnd();

    /*
    for (const auto &thread: instance->_threads)
        if (thread.get_id() == id)
            return true;

    return false;
    */
}

void Scheduler::joinThread(volatile bool &hasFinish)
{
    // if we are on a thread of the Scheduler we need to create another thread that is
    // executing another task in generic lists
    if (isExecutionSchedulerThread()) {
        for (;;) {
            {
                if (hasFinish)
                    return;
            }

            //print("Didn't finish yet " << std::this_thread::get_id() << std::endl);

            instance->execute<50>();
        }
    }
}

template <int timeWait>
auto Scheduler::execute() -> bool
{
    if constexpr (timeWait == -1)
        _semGeneral.acquire();
    else
        if (not _semGeneral.acquireWithTime(timeWait))
            return false;

    if (this->needToDie())
        return true;

    Scheduler::Ptr<WTask> task;

    {
        WMutexLocker _(_lockGeneric);
        task = _task_General.takeFirst();
    }

    //print("Start execution in thread " << std::this_thread::get_id() << std::endl);

    Scheduler::manageExecution(std::move(task));

    return false;
}

void Scheduler::manageExecution(Ptr<WTask> task)
{
    WDebug(debug and false, "Execute object" << static_cast<const void*>(&(*task)));

    const auto needToDeleteLater = task->isDeleteLater();

    task->run();

    if (needToDeleteLater) {
        task->releaseJoiner();
        task.release();
    } else {
        task.doAndUnref([](WTask& t) {
            t.releaseJoiner();
        });
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
            t.release();
    }
    _lockGeneric.unlock();

    instance = nullptr;
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

