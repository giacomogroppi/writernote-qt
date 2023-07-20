#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler()
    : WObject(nullptr)
    , _threads()
    , _semGeneral(0)
    , _semMain(0)
    , _need_to_sort(false)
    , _needToDie(false)
    , _timersWaiting([](const WTimer *t1, const WTimer *t2)-> bool {
        return t1->getEnd() >= t2->getEnd();
    })
{
    W_ASSERT(instance == nullptr);

    instance = this;
    // TODO --> change this parameter
    _threads.reserve(8);
    std::vector<std::thread> thread;

    for (int i = 0; i < 8; i++) {
        _threads.append(std::thread([this, i]() {
            const auto isMainThread = i == 0;

            WSemaphore *sem =           isMainThread ? &this->_semMain      : &this->_semGeneral;
            WMutex *mux =               isMainThread ? &this->_lockMain     : &this->_lockGeneric;
            WList<WTask *> *tasksHeap = isMainThread ? &this->_task_Main    : &this->_task_General;

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

                const auto needToDeleteLater = task->isDeleteLater();

                task->run();
                task->releaseJoiner();

                if (needToDeleteLater) {
                    delete task;
                }
            }
        }));
    }

    // start a new threads for timers
    _threads.append(std::thread([this]() {
        using namespace std;
        using namespace chrono;

        for (;;) {
            using namespace std::chrono;
            std::unique_lock<std::mutex> lk (this->_muxTimers);

            const auto shouldWaitFor = (_timersWaiting.isEmpty()
                    ? std::chrono::milliseconds (100'000).count()
                    : this->_timersWaiting.getFirst()->getDuration());

            //const auto lastValueEnd = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            const auto lastValueEnd = _timersWaiting.getFirst()->getEnd();

            this->_c.wait_for(lk, std::chrono::milliseconds (shouldWaitFor), [&lastValueEnd, this]{
                // if the list has been modified we need to reschedule the
                // timer for std::condition_value, or we need to die
                return this->_timersWaiting.getFirst()->getEnd() != lastValueEnd || needToDie();
            });

            if (needToDie())
                return;

            if (_timersWaiting.getFirst()->getEnd() < lastValueEnd) {
                // we need to wait
                continue;
            }

            auto *timer = _timersWaiting.takeFirst();

            // mutex is already locked
            Scheduler::addTaskMainThread(new WTaskFunction(nullptr, [timer]() {
                timer->trigger();
            }, true));

            if (not timer->isSingleShot()) {
                Scheduler::getInstance().addTimerUnsafe(timer);
            }
        }
    }));
}

Scheduler::~Scheduler()
{
    _needToDieLock.lock();
    this->_needToDie = true;

    this->_semMain.release(1);
    this->_semGeneral.release(_threads.size());

    _needToDieLock.unlock();

    for (auto &ref: _threads)
        ref.join();

    for (auto *t: this->_task_General)
        t->releaseJoiner();

    for (auto *t: this->_task_Main)
        t->releaseJoiner();

    instance = nullptr;
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
    W_ASSERT(this->is_heap());
}

bool Scheduler::is_heap() const
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

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);

    WMutexLocker _(instance->_lockMain);
    instance->_task_Main.append(task);

    instance->_semMain.release();
}

Scheduler &Scheduler::getInstance()
{
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
    return this->addTimerUnsafe(timer);
}

auto Scheduler::removeTimer(WTimer *timer) -> void
{
    W_ASSERT(timer != nullptr);
    WMutexLocker _(this->_muxTimers);
    if (_timersWaiting.removeIfPresent(timer))
        this->_c.notify_all();
}
