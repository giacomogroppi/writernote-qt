#include "Scheduler.h"
#include "core/WMutexLocker.h"
#include "utils/threadcount.h"

static Scheduler *instance;

Scheduler::Scheduler()
    : WObject(nullptr)
    , _need_to_sort(false)
    , _threads()
{
    W_ASSERT(instance == nullptr);

    instance = this;
    // TODO --> change this parameter
    _threads.reserve(8);
    std::vector<std::thread> thread;

    for (int i = 0; i < 8; i++) {
        _threads.append(std::thread([this, i]() {
            const auto isMainThread = i == 0;

            WSemaphore *sem = isMainThread ? &this->_semMain : &this->_semGeneral;
            WMutex *mux = isMainThread ? &this->_lockMain : &this->_lock_heap;
            WList<WTask *> *tasksHeap = isMainThread ? &this->_task_Main : &_task_General;

            // loop
            for (;;) {
                WTask *task;
                sem->acquire();

                if (this->needToDie())
                    return;

                mux->lock();
                task = tasksHeap->takeFirst();
                mux->unlock();

                task->run();
                task->releaseJoiner();
            }
        }));
    }
}

Scheduler::~Scheduler() = default;

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

void Scheduler::addTaskGeneric(WTask *task)
{
    WMutexLocker _(_lock_heap);
    this->_task_General.append(task);
    this->_need_to_sort = true;
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

constexpr bool Scheduler::needToDie() const noexcept
{
    return this->_needToDie;
}

void Scheduler::addTaskMainThread(WTask *task)
{
    W_ASSERT(task);

    WMutexLocker _(instance->_lockMain);
    instance->_task_Main.append(task);
    instance->_semMain.release();
}

