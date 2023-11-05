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
    , _conditionalVariableTasks()
    , _needToDie(false)
    , _timersWaiting([](const WTimer *t1, const WTimer *t2) -> bool {
        return t1->getEnd() > t2->getEnd();
    })
    , _conditionalVariableTimers()
{
    WDebug(debug and false, "Call constructor");

    const auto nThreads = numberOfThread();
    W_ASSERT(instance == nullptr);

    instance = this;

    _threads.reserve(nThreads + 1);
    _idThreads.reserve(nThreads);

    const auto functionThread = [this] {
        for (;;)
            if (this->execute())
                return;
    };

    for (unsigned i = 0u; i < nThreads; i++) {
        _threads.append(std::thread(functionThread));
        _idThreads.append(_threads.at(i).get_id());
    }

    // start a new threads for timers
    _threads.append(std::thread(&Scheduler::timerFunction, this));

    initMainThread();

    WAbstractList::sort(_idThreads.begin(), _idThreads.end());

    WDebug(debug and false, "finish constructor");
}

auto Scheduler::timerFunction() -> void
{
    using namespace std;
    using namespace chrono;

    using namespace std::chrono;
    std::unique_lock lk (this->_muxTimers);

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
            const auto shouldExit = [this, lastValueEnd] {
                return needToDie() or
                       (_timersWaiting.size() > 0 and _timersWaiting.getFirst()->getEnd() < lastValueEnd);
            };

            _conditionalVariableTimers.wait_for(lk, shouldWaitFor, shouldExit);
        }

        if (needToDie())
            return;

        if (_timersWaiting.isEmpty())
            continue;

        if (_timersWaiting.getFirst()->getEnd() + minWait > WTimer::now()) {
            // if the current less timer starts within minWait ms we simply execute it now
            WDebug(debug, "Timer bigger than 50ms");
            continue;
        }

        WDebug(debug, "Wake up!" << _timersWaiting.getFirst()->getDuration()
                                 << _timersWaiting.getFirst()->getEnd() << "size: " << _timersWaiting.size());

        auto timer = _timersWaiting.takeFirst();

        timer->_lock.lock();

        WDebug(debug, "Execute" << timer->getDuration());

        const auto isExecutionMainThread = timer->isExecutionMainThread();
        const auto isSingleShot = timer->isSingleShot();

        timer->_lock.unlock();

        auto task = Scheduler::Ptr<WTaskFunction>::make(nullptr, WTask::DeleteLater, [timer, isSingleShot] {
            timer->trigger();

            if (isSingleShot)
                timer->setActive(false);
            else
                timer->start();
        });

        auto method = (isExecutionMainThread) ? &Scheduler::addTaskMainThread : &Scheduler::addTaskGeneric;
        method(std::move(task));
    }
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

void Scheduler::joinThread(AtomicSafe<bool> &hasFinish)
{
    // if we are on a thread of the Scheduler we need to create another thread that is
    // executing another task in generic lists
    if (isExecutionSchedulerThread()) {
        for (;;) {
            if (hasFinish)
                return;

            //print("Didn't finish yet " << std::this_thread::get_id() << std::endl);

            instance->execute<50>();
        }
    }
}

template <int timeWait>
auto Scheduler::execute() -> bool
{
    Scheduler::Ptr<WTask> task;

    auto shouldExit = [this] {
        if (_task_General.size() > 0)
            return true;
        if (needToDie())
            return true;
        return false;
    };

    {
        std::unique_lock guard(_lockGeneric);

        if constexpr (timeWait == -1)
            _conditionalVariableTasks.wait(guard, shouldExit);
        else
            if (not _conditionalVariableTasks.wait_for(guard, std::chrono::milliseconds(timeWait), shouldExit))
                return false;

        if (this->needToDie())
            return true;

        task = _task_General.takeFirst();
    }

    //print("Start execution in thread " << std::this_thread::get_id() << std::endl);

    Scheduler::manageExecution(std::move(task));

    return false;
}

void Scheduler::manageExecution(Ptr<WTask> &&task)
{
    WDebug(debug, "Execute object" << static_cast<const void*>(&(*task)) << "with" << task.numberOfRef() << "ref");

    const auto needToDeleteLater = task->isDeleteLater();
    const auto numberOfRed = task.numberOfRef();

    if (needToDeleteLater)
        W_ASSERT(numberOfRed == 1);

    task->run();

    if (needToDeleteLater) {
        task->releaseJoiner();
        task.release();
    } else {
        task.doAndUnref(&WTask::releaseJoiner);
    }
}

Scheduler::~Scheduler()
{
    WDebug(debug and false, "Call destructor");
    this->_needToDie = true;

    {
        std::unique_lock guard (_muxTimers);
        _conditionalVariableTimers.notify_all();
    }

    {
        std::unique_lock guard(_lockGeneric);
        _conditionalVariableTasks.notify_all();
    }

    _threads.forAll(&std::thread::join);

    _lockGeneric.lock();
    for (auto &t: this->_task_General) {
        if (t->isDeleteLater()) {
            t->releaseJoiner();
            t.release();
        } else {
            t.doAndUnref(&WTask::releaseJoiner);
        }
    }
    _lockGeneric.unlock();

    endMainThread();

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
    _conditionalVariableTimers.notify_all();
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
        this->_conditionalVariableTimers.notify_all();
}

bool Scheduler::isStop() noexcept
{
    return instance == nullptr;
}

